#include <cxcore.h>
#include <cxmisc.h>
#include <cvtypes.h>
#include <hdf5.h>

CV_IMPL int cvSaveHDF5(const char * fname, const CvArr * arr){
	int ret = 0;
	CV_FUNCNAME( "cvSaveHDF5" );
	
	__BEGIN__;

	CvMat * mat = (CvMat *) arr;
	CvMat matStub;
	if( !CV_IS_MAT( mat ) ){
		mat = cvGetMat(mat, &matStub);
	}
	
	// dimensions sub-element
	int cn = CV_MAT_CN( mat->type );
	int width = mat->cols;
	int height = mat->rows;
	hsize_t dims[3] = {height, width, cn};
    int ndim = cn > 1 ? 3 : 2;

	hid_t       file, dataset;         /* file and dataset handles */
    hid_t       datatype, dataspace, type_id;   /* handles */
    herr_t      status;


	// determine datatype
	switch( CV_MAT_DEPTH(mat->type) ){
	case CV_8S:
    	type_id = H5T_NATIVE_CHAR;
		break;
	case CV_8U:
    	type_id = H5T_NATIVE_UCHAR;
		break;
	case CV_16S:
    	type_id = H5T_NATIVE_SHORT;
		break;
	case CV_16U:
    	type_id = H5T_NATIVE_USHORT;
		break;
	case CV_32S:
    	type_id = H5T_NATIVE_INT;
		break;
	case CV_32F:
    	type_id = H5T_NATIVE_FLOAT;
		break;
	case CV_64F:
    	type_id = H5T_NATIVE_DOUBLE;
		break;
	default:
		CV_ERROR( CV_StsUnsupportedFormat, "Could not determine type of matrix, it may be corrupt." );
	}

	/*
     * Create a new file using H5F_ACC_TRUNC access,
     * default file creation properties, and default file
     * access properties.
     */
    file = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Describe the size of the array and create the data space for fixed
     * size dataset. 
     */
    dataspace = H5Screate_simple(ndim, dims, NULL);

	/* 
     * Define datatype for the data in the file.
	 * Store little-endian
     */
    datatype = H5Tcopy(type_id);
    status = H5Tset_order(datatype, H5T_ORDER_LE);

    /*
     * Create a new dataset within the file using defined dataspace and
     * datatype and default dataset creation properties.
     */
    dataset = H5Dcreate(file, "cvarr", datatype, dataspace,
            H5P_DEFAULT);

    /*
     * Write the data to the dataset using default transfer properties.
     */
    status = H5Dwrite(dataset, type_id, H5S_ALL, H5S_ALL,
              H5P_DEFAULT, mat->data.ptr);

    /*
     * Close/release resources.
     */
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Fclose(file);


	__END__;
	return ret;
}

#ifdef MAIN
#include <highgui.h>
int main(int argc, char ** argv){
	IplImage * im = cvLoadImage(argv[1], 0);
	CvMat * mat = cvCreateMat(im->height, im->width, CV_64F);
	cvScale(im,mat);
	cvSaveHDF5("A.h5", mat);
}
#endif
