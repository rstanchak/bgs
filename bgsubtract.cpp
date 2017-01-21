#include <cvaux.h>
#include <highgui.h>

 
CvGaussBGStatModelParams params1;
CvFGDStatModelParams params2;

int main(int argc, char ** argv){

	CvCapture * cap = cvCreateFileCapture( argv[1] );
	assert(cap);

	// gaussian mixture parameters
	params1.win_size = 30; //CV_BGFG_MOG_WINDOW_SIZE;
	params1.bg_threshold = 0.01; //CV_BGFG_MOG_BACKGROUND_THRESHOLD;
	params1.std_threshold = 2.5; //CV_BGFG_MOG_STD_THRESHOLD;
	params1.weight_init = 0.05; //CV_BGFG_MOG_WEIGHT_INIT;
	params1.variance_init = 30*30; //CV_BGFG_MOG_SIGMA_INIT*CV_BGFG_MOG_SIGMA_INIT;
	params1.minArea = 25.f; //CV_BGFG_MOG_MINAREA;
	params1.n_gauss = 5; //CV_BGFG_MOG_NGAUSSIANS;

	// fg parameters
	params2.Lc = CV_BGFG_FGD_LC;
	params2.N1c = CV_BGFG_FGD_N1C;
	params2.N2c = CV_BGFG_FGD_N2C;
	params2.Lcc = CV_BGFG_FGD_LCC;
	params2.N1cc = CV_BGFG_FGD_N1CC;
	params2.N2cc = CV_BGFG_FGD_N2CC;
	params2.delta = CV_BGFG_FGD_DELTA;
	params2.alpha1 = CV_BGFG_FGD_ALPHA_1;
	params2.alpha2 = CV_BGFG_FGD_ALPHA_2;
	params2.alpha3 = CV_BGFG_FGD_ALPHA_3;
	params2.T = CV_BGFG_FGD_T;
	params2.minArea = CV_BGFG_FGD_MINAREA*.5;
	params2.is_obj_without_holes = 1;
	params2.perform_morphing = 1;



	IplImage * im = cvQueryFrame(cap);
	IplImage * smallim = cvCreateImage(cvSize(192, 192*im->height/im->width), 8, 3);
	cvResize(im, smallim, CV_INTER_AREA);
	//CvBGStatModel * bgmodel = cvCreateFGDStatModel( smallim, &params2 );
	CvBGStatModel * bgmodel = cvCreateGaussianBGModel( smallim, &params1 );
	CvGaussBGModel * gauss = (CvGaussBGModel *) bgmodel;

	cvNamedWindow("orig",0);
	cvNamedWindow("fg",0);
	cvNamedWindow("prob",0);
	cvMoveWindow("orig", 0,0);
	cvMoveWindow("prob", 512, 0);
	cvResizeWindow("orig", 512, 512*im->height/im->width);
	cvResizeWindow("prob", 512, 512*im->height/im->width);


	CvFont font = cvFont(1.0);
	char str[256];
	for(int i=0; true; i++){
		im = cvQueryFrame(cap);
		cvResize(im, smallim, CV_INTER_AREA);
		cvUpdateBGStatModel( smallim, bgmodel );
		cvShowImage("fg", bgmodel->foreground);
		cvShowImage("prob", gauss->prob );

		sprintf(str, "bg%04d.bmp", i);
		//cvSaveImage(str, bgmodel->foreground);
		sprintf(str, "fg%04d.bmp", i);
		//cvSaveImage(str, im); 

		sprintf(str, "Frame %d", i+1);
		//cvPutText(im, str, cvPoint(0, 10), &font, CV_RGB(0,0,0));
		cvShowImage("orig", im);
		if(cvWaitKey(33)=='q') break;
	}

}
