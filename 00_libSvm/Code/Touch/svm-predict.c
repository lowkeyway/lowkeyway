#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>
#include <limits.h>
#include <stdbool.h>
#include "svm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

static int (*info)(const char *fmt,...) = &printf;

struct svm_node *x;
int max_nr_attr = 64;

struct svm_model* model;

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

static int svm_get_svm_type(const struct svm_model* model)
{
	return model->param.svm_type;
}

int svm_get_nr_class(const struct svm_model* model)
{
	return model->nr_class;
}

double k_function(const struct svm_node* x, const struct svm_node* y,
	const struct svm_parameter param)
{
	switch (param.kernel_type)
	{
	case RBF:
	{
		double sum = 0;
		while (x->index != -1 && y->index != -1)
		{
			if (x->index == y->index)
			{
				double d = x->value - y->value;
				sum += d * d;
				++x;
				++y;
			}
			else
			{
				if (x->index > y->index)
				{
					sum += y->value * y->value;
					++y;
				}
				else
				{
					sum += x->value * x->value;
					++x;
				}
			}
		}

		while (x->index != -1)
		{
			sum += x->value * x->value;
			++x;
		}

		while (y->index != -1)
		{
			sum += y->value * y->value;
			++y;
		}

		return exp(-param.gamma * sum);
	}
	default:
		return 0;  // Unreachable
	}
}

double svm_predict_values(const struct svm_model* model, const struct svm_node* x, double* dec_values)
{
	int i;

	{
		int nr_class = model->nr_class;
		int l = model->l;

		double* kvalue = Malloc(double, l);
		for (i = 0; i < l; i++)
			kvalue[i] = k_function(x, model->SV[i], model->param);

		int* start = Malloc(int, nr_class);
		start[0] = 0;
		for (i = 1; i < nr_class; i++)
			start[i] = start[i - 1] + model->nSV[i - 1];

		int* vote = Malloc(int, nr_class);
		for (i = 0; i < nr_class; i++)
			vote[i] = 0;

		int p = 0;
		for (i = 0; i < nr_class; i++)
			for (int j = i + 1; j < nr_class; j++)
			{
				double sum = 0;
				int si = start[i];
				int sj = start[j];
				int ci = model->nSV[i];
				int cj = model->nSV[j];

				int k;
				double* coef1 = model->sv_coef[j - 1];
				double* coef2 = model->sv_coef[i];
				for (k = 0; k < ci; k++)
					sum += coef1[si + k] * kvalue[si + k];
				for (k = 0; k < cj; k++)
					sum += coef2[sj + k] * kvalue[sj + k];
				sum -= model->rho[p];
				dec_values[p] = sum;

				if (dec_values[p] > 0)
					++vote[i];
				else
					++vote[j];
				p++;
			}

		int vote_max_idx = 0;
		for (i = 1; i < nr_class; i++)
			if (vote[i] > vote[vote_max_idx])
				vote_max_idx = i;

		free(kvalue);
		free(start);
		free(vote);
		return model->label[vote_max_idx];
	}
}

double svm_predict(const struct svm_model* model, const struct svm_node* x)
{
	int nr_class = model->nr_class;
	double* dec_values;

	dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
	double pred_result = svm_predict_values(model, x, dec_values);
	free(dec_values);
	return pred_result;
}

void exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	exit(1);
}



void predict(FILE *input, FILE *output)
{
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);
	int j;

	max_line_len = 2048;
	line = (char *)malloc(max_line_len*sizeof(char));
	while(readline(input) != NULL)
	{
		int i = 0;
		double target_label, predict_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0

		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(total+1);

		target_label = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(total+1);

		while(1)
		{
			if(i>=max_nr_attr-1)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct svm_node *) realloc(x,max_nr_attr*sizeof(struct svm_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
				exit_input_error(total+1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(total+1);

			++i;
		}
		x[i].index = -1;

		{
			predict_label = svm_predict(model,x);
			fprintf(output,"%.17g\n",predict_label);
		}

		if(predict_label == target_label)
			++correct;
		error += (predict_label-target_label)*(predict_label-target_label);
		sump += predict_label;
		sumt += target_label;
		sumpp += predict_label*predict_label;
		sumtt += target_label*target_label;
		sumpt += predict_label*target_label;
		++total;
	}

	info("Accuracy = %g%% (%d/%d) (classification)\n",
		(double)correct/total*100,correct,total);
}

void dump_svm_model(struct svm_model* model)
{
#if 0
	struct svm_parameter
	{
		int svm_type;
		int kernel_type;
		int degree;	/* for poly */
		double gamma;	/* for poly/rbf/sigmoid */
		double coef0;	/* for poly/sigmoid */

		/* these are for training only */
		double cache_size; /* in MB */
		double eps;	/* stopping criteria */
		double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
		int nr_weight;		/* for C_SVC */
		int* weight_label;	/* for C_SVC */
		double* weight;		/* for C_SVC */
		double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
		double p;	/* for EPSILON_SVR */
		int shrinking;	/* use the shrinking heuristics */
		int probability; /* do probability estimates */
	};

	//
	// svm_model
	//
	struct svm_model
	{
		struct svm_parameter param;	/* parameter */
		int nr_class;		/* number of classes, = 2 in regression/one class svm */
		int l;			/* total #SV */
		struct svm_node** SV;		/* SVs (SV[l]) */
		double** sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
		double* rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
		double* probA;		/* pariwise probability information */
		double* probB;
		int* sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

		/* for classification only */

		int* label;		/* label of each class (label[k]) */
		int* nSV;		/* number of SVs for each class (nSV[k]) */
					/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
		/* XXX */
		int free_sv;		/* 1 if svm_model is created by svm_load_model*/
					/* 0 if svm_model is created by svm_train */
	};
#endif
	int i = 0, j = 0;

	info("param.svm_type = %d\n", model->param.svm_type);
	info("param.kernel_type = %d\n", model->param.kernel_type);
	info("param.degree = %d\n", model->param.degree);
	info("param.gamma = %f\n", model->param.gamma);
	info("param.coef0 = %f\n", model->param.coef0);
	info("param.cache_size = %f\n", model->param.cache_size);
	info("param.eps = %f\n", model->param.eps);
	info("param.C = %f\n", model->param.C);
	info("param.nu = %d\n", model->param.nu);
	info("param.p = %f\n", model->param.p);
	info("param.shrinking = %d\n", model->param.shrinking);
	info("param.probability = %d\n", model->param.probability);

	info("nr_class = %d\n", model->nr_class);
	info("l = %d\n", model->l);
	info("free_sv = %d\n", model->free_sv);
	info("rho = %f\n", model->rho[0]);
	info("label = [%d, %d]\n", model->label[0], model->label[1]);
	info("nSV = [%d, %d]\n", model->nSV[0], model->nSV[1]);

	info("sv_coef = ");
	for (i = 0; i < model->l; i++)
		info("%f ", model->sv_coef[0][i]);
	info("\n");

#if 0
	info("SV = \n");
	for (i = 0; i < model->l; i++, j = 0)
	{
		while (-1 != model->SV[i][j].index)
		{
			info("%d:%f ", model->SV[i][j].index, model->SV[i][j].value);
			j++;
		}

		info("\n");
	}
#endif
}


//
// FSCANF helps to handle fscanf failures.
// Its do-while block avoids the ambiguity when
// if (...)
//    FSCANF();
// is used
//
static const char* svm_type_table[] =
{
	"c_svc","nu_svc","one_class","epsilon_svr","nu_svr",NULL
};

static const char* kernel_type_table[] =
{
	"linear","polynomial","rbf","sigmoid","precomputed",NULL
};

#define FSCANF(_stream, _format, _var) do{ if (fscanf(_stream, _format, _var) != 1) return false; }while(0)
static bool read_model_header(FILE* fp, struct svm_model* model)
{
	struct svm_parameter *param = &model->param;
	// parameters for training only won't be assigned, but arrays are assigned as NULL for safety
	param->nr_weight = 0;
	param->weight_label = NULL;
	param->weight = NULL;

	char cmd[81];
	while (1)
	{
		FSCANF(fp, "%80s", cmd);
		LOU_P("%s\n", cmd);
		if (strcmp(cmd, "svm_type") == 0)
		{
			FSCANF(fp, "%80s", cmd);
			int i;
			for (i = 0; svm_type_table[i]; i++)
			{
				if (strcmp(svm_type_table[i], cmd) == 0)
				{
					param->svm_type = i;
					break;
				}
			}
			if (svm_type_table[i] == NULL)
			{
				fprintf(stderr, "unknown svm type.\n");
				return false;
			}
		}
		else if (strcmp(cmd, "kernel_type") == 0)
		{
			FSCANF(fp, "%80s", cmd);
			int i;
			for (i = 0; kernel_type_table[i]; i++)
			{
				if (strcmp(kernel_type_table[i], cmd) == 0)
				{
					param->kernel_type = i;
					break;
				}
			}
			if (kernel_type_table[i] == NULL)
			{
				fprintf(stderr, "unknown kernel function.\n");
				return false;
			}
		}
		else if (strcmp(cmd, "degree") == 0)
			FSCANF(fp, "%d", &param->degree);
		else if (strcmp(cmd, "gamma") == 0)
			FSCANF(fp, "%lf", &param->gamma);
		else if (strcmp(cmd, "coef0") == 0)
			FSCANF(fp, "%lf", &param->coef0);
		else if (strcmp(cmd, "nr_class") == 0)
			FSCANF(fp, "%d", &model->nr_class);
		else if (strcmp(cmd, "total_sv") == 0)
			FSCANF(fp, "%d", &model->l);
		else if (strcmp(cmd, "rho") == 0)
		{
			int n = model->nr_class * (model->nr_class - 1) / 2;
			model->rho = Malloc(double, n);
			for (int i = 0; i < n; i++)
				FSCANF(fp, "%lf", &model->rho[i]);
		}
		else if (strcmp(cmd, "label") == 0)
		{
			int n = model->nr_class;
			model->label = Malloc(int, n);
			for (int i = 0; i < n; i++)
				FSCANF(fp, "%d", &model->label[i]);
		}
		else if (strcmp(cmd, "probA") == 0)
		{
			int n = model->nr_class * (model->nr_class - 1) / 2;
			model->probA = Malloc(double, n);
			for (int i = 0; i < n; i++)
				FSCANF(fp, "%lf", &model->probA[i]);
		}
		else if (strcmp(cmd, "probB") == 0)
		{
			int n = model->nr_class * (model->nr_class - 1) / 2;
			model->probB = Malloc(double, n);
			for (int i = 0; i < n; i++)
				FSCANF(fp, "%lf", &model->probB[i]);
		}
		else if (strcmp(cmd, "nr_sv") == 0)
		{
			int n = model->nr_class;
			model->nSV = Malloc(int, n);
			for (int i = 0; i < n; i++)
				FSCANF(fp, "%d", &model->nSV[i]);
		}
		else if (strcmp(cmd, "SV") == 0)
		{
			while (1)
			{
				int c = getc(fp);
				if (c == EOF || c == '\n') break;
			}
			break;
		}
		else
		{
			fprintf(stderr, "unknown text in model file: [%s]\n", cmd);
			return false;
		}
	}

	return true;

}

static void fill_model(struct svm_model* model)
{
	model->param.svm_type = 0;
	model->param.kernel_type = 2;
	model->param.gamma = 0.0078120000000000004;
	model->nr_class = 2;
	model->l = 19;
	model->rho = Malloc(double, 1);
	model->rho[0] = -0.0018908804852536274;
	model->label = Malloc(int, 2);
	model->label[0] = 1;
	model->label[1] = -1;
	model->nSV = Malloc(int, 2);
	model->nSV[0] = 10;
	model->nSV[1] = 9;
}

static struct svm_model* svm_load_model(const char* model_file_name)
{
	FILE* fp = fopen(model_file_name, "rb");
	if (fp == NULL) return NULL;

	char* old_locale = setlocale(LC_ALL, NULL);
	if (old_locale) {
		old_locale = strdup(old_locale);
	}
	setlocale(LC_ALL, "C");

	// read parameters

	struct svm_model* model = Malloc(struct svm_model, 1);
	model->rho = NULL;
	model->probA = NULL;
	model->probB = NULL;
	model->sv_indices = NULL;
	model->label = NULL;
	model->nSV = NULL;


	// read header
	if (!read_model_header(fp, model))
	{
		fprintf(stderr, "ERROR: fscanf failed to read model\n");
		setlocale(LC_ALL, old_locale);
		free(old_locale);
		free(model->rho);
		free(model->label);
		free(model->nSV);
		free(model);
		return NULL;
	}

	// read sv_coef and SV
	fill_model(model);

	int elements = 0;
	long pos = ftell(fp);
	LOU_P("pos = %ld\n", pos);
	max_line_len = 2048;
	line = Malloc(char, max_line_len);
	char* p, * endptr, * idx, * val;

	while (readline(fp) != NULL)
	{
		//LOU_P("elements[%d]: %s\n", elements, line);
		p = strtok(line, ":");
		if (160 > elements) LOU_P("%s\n", p);
		while (1)
		{
			p = strtok(NULL, ":");
			if (p == NULL)
				break;
			++elements;
			if (160 > elements) LOU_P("%s\n", p);
		}

	}
	elements += model->l;

	fseek(fp, pos, SEEK_SET);

	int m = model->nr_class - 1;
	int l = model->l;
	model->sv_coef = Malloc(double*, m);
	int i;
	for (i = 0; i < m; i++)
		model->sv_coef[i] = Malloc(double, l);
	model->SV = Malloc(struct svm_node*, l);
	struct svm_node* x_space = NULL;
	if (l > 0) x_space = Malloc(struct svm_node, elements);

	int j = 0;
	for (i = 0; i < l; i++)
	{
		readline(fp);
		model->SV[i] = &x_space[j];

		p = strtok(line, " \t");
		model->sv_coef[0][i] = strtod(p, &endptr);
		for (int k = 1; k < m; k++)
		{
			p = strtok(NULL, " \t");
			model->sv_coef[k][i] = strtod(p, &endptr);
		}

		while (1)
		{
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if (val == NULL)
				break;
			x_space[j].index = (int)strtol(idx, &endptr, 10);
			x_space[j].value = strtod(val, &endptr);

			++j;
		}
		x_space[j++].index = -1;
	}
	free(line);

	setlocale(LC_ALL, old_locale);
	free(old_locale);

	if (ferror(fp) != 0 || fclose(fp) != 0)
		return NULL;

	model->free_sv = 1;	// XXX
	return model;
}


void svm_free_model_content(struct svm_model* model_ptr)
{
	if (model_ptr->free_sv && model_ptr->l > 0 && model_ptr->SV != NULL)
		free((void*)(model_ptr->SV[0]));
	if (model_ptr->sv_coef)
	{
		for (int i = 0; i < model_ptr->nr_class - 1; i++)
			free(model_ptr->sv_coef[i]);
	}

	free(model_ptr->SV);
	model_ptr->SV = NULL;

	free(model_ptr->sv_coef);
	model_ptr->sv_coef = NULL;

	free(model_ptr->rho);
	model_ptr->rho = NULL;

	free(model_ptr->label);
	model_ptr->label = NULL;

	free(model_ptr->probA);
	model_ptr->probA = NULL;

	free(model_ptr->probB);
	model_ptr->probB = NULL;

	free(model_ptr->sv_indices);
	model_ptr->sv_indices = NULL;

	free(model_ptr->nSV);
	model_ptr->nSV = NULL;
}

static void svm_free_and_destroy_model(struct svm_model** model_ptr_ptr)
{
	if (model_ptr_ptr != NULL && *model_ptr_ptr != NULL)
	{
		svm_free_model_content(*model_ptr_ptr);
		free(*model_ptr_ptr);
		*model_ptr_ptr = NULL;
	}
}


int main(int argc, char **argv)
{
	FILE *input, *output;
	int i;

	i = 1;
	LOU_P("i: %d, Argc: %d\n", i, argc);

	if (argc != 4)
	{
		info("Incorrect input\n");
		return 0;
	}
		

	input = fopen(argv[i],"r");
	if(input == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",argv[i]);
		exit(1);
	}

	output = fopen(argv[i+2],"w");
	if(output == NULL)
	{
		fprintf(stderr,"can't open output file %s\n",argv[i+2]);
		exit(1);
	}

	if((model=svm_load_model(argv[i+1]))==0)
	{
		fprintf(stderr,"can't open model file %s\n",argv[i+1]);
		exit(1);
	}
	dump_svm_model(model);
	x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));

	predict(input,output);
	svm_free_and_destroy_model(&model);
	free(x);
	free(line);
	fclose(input);
	fclose(output);
	return 0;
}
