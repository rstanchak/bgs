#include <unistd.h>
#include <cvaux.h>
#include <highgui.h>
#include <_highgui.h>

 
CvGaussBGStatModelParams params1;
CvFGDStatModelParams params2;

void usage(char * progname){
	fprintf(stderr, "USAGE %s <movie file> <start frame> <num frames> <output directory>\n", progname);
}

int main(int argc, char ** argv){

	if(argc<3){
		usage(argv[0]);
		return -1;
	}

	CvCapture * cap = cvCreateFileCapture( argv[1] );
	assert(cap);
//	int start_frame = atoi( argv[2] );
//	assert(start_frame >= 0);
    int num_frames = atoi( argv[2] );
	char * target_dir = argv[3];


	// gaussian mixture parameters
	params1.win_size = 25; //CV_BGFG_MOG_WINDOW_SIZE;
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


	/*
	printf("Seeking to start_frame=%d\n", start_frame);
		for(int i=0; i<start_frame; i++){
		cvGrabFrame(cap);
		if(i%60==0) printf("Frame %d of %d\n", i, start_frame);
	}*/
	//cvSetCaptureProperty( cap, CV_CAP_PROP_POS_FRAMES, start_frame );
	//assert(cvGetCaptureProperty( cap, CV_CAP_PROP_POS_FRAMES )==start_frame);

	IplImage * im = cvQueryFrame(cap);
	cvNamedWindow("orig",0);
	cvShowImage("orig", im);
	//cvWaitKey();

	//CvBGStatModel * bgmodel = cvCreateFGDStatModel( smallim, &params2 );
	CvBGStatModel * bgmodel = cvCreateGaussianBGModel( im, &params1 );
	CvGaussBGModel * gauss = (CvGaussBGModel *) bgmodel;

	CvMat * prob8u = cvCreateMat( im->height, im->width, CV_8U );


	char str[256];
	int total_frames = (int) cvGetCaptureProperty( cap, CV_CAP_PROP_FRAME_COUNT );
	cvSetErrMode( CV_ErrModeSilent );
	for(int i=0; i<num_frames; i++){
		bool haveerror=true;
		im = cvRetrieveFrame(cap);
		if(true){
			cvUpdateBGStatModel( im, bgmodel );

			haveerror=true;
			do{
				sprintf(str, "%s/fg/fg%05d.bmp", target_dir, i);
				cvSaveImage(str, bgmodel->foreground);

	//			sprintf(str, "%s/img/im%05d.bmp", target_dir, i);
	//			cvSaveImage(str, im); 
		
				sprintf(str, "%s/pfg/p%05d.bmp", target_dir, i);
				cvScale(gauss->prob, prob8u, 255, 0);
				cvSaveImage(str, prob8u);

				if( cvGetErrStatus() < 0 ){
					printf("Error saving images -- do you need to run chkdsk on the stupid NTFS partition?\n");
					cvWaitKey(-1);
				}
				else{
					haveerror=false;
				}

			} while(haveerror);


			cvShowImage("orig", im);
			printf("Frame %d of %d\n", i, total_frames);
			if(cvWaitKey(33)=='q') break;
		}
		do{
			cvGrabFrame(cap);
			haveerror=true;
			if( cvGetErrStatus() < 0 ){
				printf("Error saving images -- do you need to run chkdsk on the stupid NTFS partition?\n");
				cvWaitKey(-1);
			}
			else{
				haveerror=false;
			}
		}while(haveerror);
	}
}
