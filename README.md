# MRZ_Antitampering

This C++ application is used to detect MRZ fields in PRADO documents based on a model developed in *Python* using the libraries *Tensorflow*, *Keras*, and *OpenCV*.

The following C++ libraries have to be downloaded:
- *OpenCV 4.6.0*
- ...

The *Pugixml* library is also exploited even though the user is not required to have it installed in his machine, since the useful files are already included in the repository.

Before running the code, all the CMake files must be created through the following lines:
```
mkdir build
cd build
cmake --make ..
make
```

To extract the prediction:

    
    ./app
    
And to make unit tests:

    ./TestRunner