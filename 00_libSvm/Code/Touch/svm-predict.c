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
#include "svm_model.h"
#include "svm_predict.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

static int (*info)(const char *fmt,...) = &printf;

struct svm_node *x;
int max_nr_attr = 64 * 3;

struct svm_model* model;

static int max_line_len;

double k_function(double* x, double* y,	const struct svm_parameter param)
{
	double sum = 0;
	int i = 0;
	int len = ARRAY_SIZE(y);
	while (i++ < len)
	{
		double d = *x - *y;
		sum += d * d;
		++x;
		++y;
	}

	return exp(-param.gamma * sum);
}

double svm_predict_values(const struct svm_model* model, double* x, double* dec_values)
{
	int i;

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

double svm_predict(const struct svm_model* model, double* x)
{
	int nr_class = model->nr_class;
	double* dec_values;

	dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
	double pred_result = svm_predict_values(model, x, dec_values);
	free(dec_values);
	return pred_result;
}

void predict()
{
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int i, j = 0;

	info("sizeof array = [%ld], sizeof array[0] = [%ld]\n", ARRAY_SIZE(predict_data), ARRAY_SIZE(predict_data[0]));
	//info("sizeof array = [%ld], sizeof array[0] = [%ld]\n", sizeof(predict_data), sizeof(predict_data[0]));
	for(i = 0; i < ARRAY_SIZE(predict_data); i++, j = 0)
	{
		double target_label, predict_label;
		target_label = predict_target_label[i];
		
		{
			predict_label = svm_predict(model, predict_data[i]);
			info("%.17g\n",predict_label);
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
	int i = 0, j = 0;
	double *coef = NULL;
	double *pNode = NULL;

	info("param.svm_type = %d\n", model->param.svm_type);
	info("param.kernel_type = %d\n", model->param.kernel_type);
	info("param.degree = %d\n", model->param.degree);
	info("param.gamma = %f\n", model->param.gamma);


	info("nr_class = %d\n", model->nr_class);
	info("l = %d\n", model->l);
	info("rho = %f\n", model->rho[0]);
	info("label = [%d, %d]\n", model->label[0], model->label[1]);
	info("nSV = [%d, %d]\n", model->nSV[0], model->nSV[1]);

	info("sv_coef = ");
	coef = model->sv_coef[0];
	for (i = 0; i < model->l; i++)
		info("%f ", *(coef + i));
	info("\n");

	info("SV = ");
	for (i = 0; i < model->l; i++)
	{
		pNode = model->SV[i];
		for(j = 0; j < 10; j++)
			info("%f, ", pNode[j]);
		info("\n");
	}
		
	info("\n");
}

static struct svm_model* fill_model()
{
	struct svm_model* model = Malloc(struct svm_model, 1);
	memset(model, 0, sizeof(struct svm_model));
	int i;

	model->param.svm_type = SVM_TYPE;
	model->param.kernel_type = KERNEL_TYPE;
	model->param.gamma = GAMMA;
	model->nr_class = NR_CLASS;
	model->l = TOTAL_SV;
	model->rho = rho;
	model->label = label;

	model->nSV = nr_sv;

	int m = model->nr_class - 1;
	model->sv_coef = Malloc(double*, m);
	model->sv_coef[0] = (double*)&sv_coef[0];

	int l = model->l;
	model->SV = Malloc(double*, l);
	for(i = 0; i < l; i++)
		model->SV[i] = (double*)&SV[i];

	return model;
}



static void svm_free_and_destroy_model(struct svm_model** model_ptr_ptr)
{
	if (model_ptr_ptr != NULL && *model_ptr_ptr != NULL)
	{
		free(*model_ptr_ptr);
		*model_ptr_ptr = NULL;
	}
}


int main(int argc, char **argv)
{
	model = fill_model();
	dump_svm_model(model);

	predict();
	svm_free_and_destroy_model(&model);

	return 0;
}
