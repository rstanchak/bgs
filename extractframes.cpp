#include <unistd.h>
#include <cvaux.h>
#include <highgui.h>

 
CvGaussBGStatModelParams params1;
CvFGDStatModelParams params2;

void usage(char * progname){
	fprintf(stderr, "USAGE %s <movie file> <start frame> <num frames> <output directory>\n", progname);
}

int main(int argc, char ** argv){

	if(argc<5){
		usage(argv[0]);
		return -1;
	}

	CvCapture * cap = cvCreateFileCapture( argv[1] );
	assert(cap);
	int start_frame = atoi( argv[2] );
	assert(start_frame >= 0);
	int num_frames = atoi( argv[3] );
	char * target_dir = argv[4];

	printf("Seeking to start_frame=%d\n", start_frame);
	/*	for(int i=0; i<start_frame; i++){
		cvGrabFrame(cap);
		if(i%60==0) printf("Frame %d of %d\n", i, start_frame);
	}*/
	cvSetCaptureProperty( cap, CV_CAP_PROP_POS_FRAMES, start_frame );
	assert(cvGetCaptureProperty( cap, CV_CAP_PROP_POS_FRAMES )==start_frame);

	IplImage * im = cvQueryFrame(cap);
	assert(im);
	//cvWaitKey();

	//CvBGStatModel * bgmodel = cvCreateFGDStatModel( smallim, &params2 );

	cvNamedWindow("orig");
	cvShowImage("orig", im);
	char str[256];
	int total_frames = (int) cvGetCaptureProperty( cap, CV_CAP_PROP_FRAME_COUNT );
	if(total_frames<num_frames+start_frame){
		printf("WARNING: only %d frames to capture and requested %d\n", total_frames, num_frames+start_frame);
	}
	for(int i=0; i<num_frames; i++){
		im = cvRetrieveFrame(cap);
		if(!im) break;

		sprintf(str, "%s/img/im%05d.bmp", target_dir, i);
		cvSaveImage(str, im); 

		if(i%50==0){
			printf("Frame %d of %d\n", i, num_frames);
			cvShowImage("orig", im);
			if(cvWaitKey(33)=='q') break;
		}
		
		if(!cvGrabFrame(cap)) break;
	}
}
