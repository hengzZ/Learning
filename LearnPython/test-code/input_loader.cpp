#include <Python.h>

// Produce deprecation warnings (needs to come before arrayobject.h inclusion).
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <numpy/arrayobject.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <omp.h>

// Temporary solution for numpy < 1.7 versions: old macro, no promises.
// You're strongly advised to upgrade to >= 1.7.
#ifndef NPY_ARRAY_C_CONTIGUOUS
#define NPY_ARRAY_C_CONTIGUOUS NPY_C_CONTIGUOUS
#define PyArray_SetBaseObject(arr, x) (PyArray_BASE(arr) = (x))
#endif

namespace bp = boost::python;

// For Python, for now, we'll just always use float as the type.
typedef float Dtype;
const int NPY_DTYPE = NPY_FLOAT32;


// Check the PyArrary transformed from python into C++
void CheckContiguousArray(PyArrayObject* arr, std::string name,
    int batch, int channels, int height, int width) {
  if (!(PyArray_FLAGS(arr) & NPY_ARRAY_C_CONTIGUOUS)) {
    throw std::runtime_error(name + " must be C contiguous");
  }
  if (PyArray_NDIM(arr) != 4) {
    throw std::runtime_error(name + " must be 4-d");
  }
  if (PyArray_TYPE(arr) != NPY_FLOAT32) {
    throw std::runtime_error(name + " must be float32");
  }
  if (PyArray_DIMS(arr)[0] != batch) {
    throw std::runtime_error(name + " has wrong number of batch");
  }
  if (PyArray_DIMS(arr)[1] != channels) {
    throw std::runtime_error(name + " has wrong number of channels");
  }
  if (PyArray_DIMS(arr)[2] != height) {
    throw std::runtime_error(name + " has wrong height");
  }
  if (PyArray_DIMS(arr)[3] != width) {
    throw std::runtime_error(name + " has wrong width");
  }
}

// Convert bp object from list to vector
std::vector<std::string> ConvertBpListToVector(const bp::object& list)
{
  std::vector<std::string> string_vector;
  if (!list.is_none()) {
    for (int i = 0; i < len(list); i++) {
      string_vector.push_back(bp::extract<std::string>(list[i]));
    }
  }
  return string_vector;
}

// Convert bp object from python str to stl string
std::string ConvertBpStrToString(const bp::object& str)
{
  std::string ret_string;
  if (!str.is_none()) {
    ret_string = bp::extract<std::string>(str);
  }
  return ret_string;
}


void LoadImagesToInputArray(std::vector<std::string> image_list, int batch_size, int channels, int height, int width, Dtype* input)
{
    if (image_list.size() < 1) { return; }

    #pragma omp parallel for num_threads(4)
    for (int bidx = 0; bidx < batch_size; bidx++)
    {
        //int id = omp_get_thread_num(); printf("TID:%d\n\n",id);
        std::string imageFile = image_list[bidx%image_list.size()];
        //std::cout << bidx << " of " << image_list.size() << ":"  << imageFile << "\n" << std::endl;


        // Load image
        cv::Mat img = cv::imread(imageFile.c_str(), -1);
        if (img.channels() != channels) {
            printf("Channels does not match (image channels=%d, required channels=%d)\n",
                   img.channels(), channels);
            exit(-1);
        }

        // Resize if needed
        cv::Size inputSize(width, height);
        if (img.size() != inputSize) {
            cv::Mat resizedImg;
            cv::resize(img, resizedImg, inputSize);
            img = resizedImg;
        }

        // Convert to float data
        cv::Mat sample;
        int type = ((channels == 3) ? CV_32FC3 : CV_32FC1);
        img.convertTo(sample, type);

        // Split channels (after this, it is in CHW format)
        std::vector<cv::Mat> inputChannels;
        for (int i = 0; i < channels; ++i) {
            cv::Mat channel(height, width, CV_32FC1, input + (bidx*channels+i)*height*width);
            inputChannels.push_back(channel);
        }
        cv::split(sample, inputChannels);
    }
}


// Load images batch by batch
void LoadOneBatchImages(bp::object image_list, bp::object data_obj, int batch_size, int channels, int height, int width)
{
  // check that we were passed appropriately-sized contiguous memory
  PyArrayObject* data_arr = reinterpret_cast<PyArrayObject*>(data_obj.ptr());
  CheckContiguousArray(data_arr, "data array", batch_size, channels, height, width);
  std::vector<std::string> imageList = ConvertBpListToVector(image_list);
  // get the memory ptr
  Dtype* input = static_cast<Dtype*>(PyArray_DATA(data_arr));
  LoadImagesToInputArray(imageList, batch_size, channels, height, width, input);
}


BOOST_PYTHON_MODULE(input_loader)
{
  bp::def("load_one_batch_images", &LoadOneBatchImages);
}

