#ifndef _LIBSVM_H
#define _LIBSVM_H

#define LIBSVM_VERSION 324
#define LOU_P(_fmt, ...) printf("[LOUIS][%s][%d] "##_fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

extern int libsvm_version;

struct svm_node
{
	int index;
	double value;
};


struct svm_parameter
{
	int svm_type;
	int kernel_type;
	int degree;	/* for poly */
	double gamma;	/* for poly/rbf/sigmoid */
	double coef0;	/* for poly/sigmoid */
};

//
// svm_model
//
struct svm_model
{
	struct svm_parameter param;	/* parameter */
	int nr_class;		/* number of classes, = 2 in regression/one class svm */
	int l;			/* total #SV */
	double **SV;		/* SVs (SV[l]) */
	double **sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho;		/* constants in decision functions (rho[k*(k-1)/2]) */

	/* for classification only */

	int *label;		/* label of each class (label[k]) */
	int *nSV;		/* number of SVs for each class (nSV[k]) */
				/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
};


double svm_predict_values(const struct svm_model *model, double *x, double* dec_values);
double svm_predict(const struct svm_model *model, double *x);

#ifdef __cplusplus
}
#endif

#endif /* _LIBSVM_H */
