# MRZ_Antitampering

This C++ application is used to detect MRZ fields in PRADO documents based on a model developed in *Python* using the libraries *Tensorflow*, *Keras*, and *OpenCV*.

The *Pugixml* library is also exploited even though the user is not required to have it installed in his machine, since the useful files are already included in the repository.

To build the application runs:

Before running the code, all the CMake files must be created through the following lines:
```
conan install . --build=missing
conan build .
```

Conan generate the executables for the application and for the tests in the folder *build\Release*, so move into this folder with:
```
cd build\Release
```

Then to extract the prediction run:
```
./application
```
    
And to make unit tests:
```
./TestExecutable
```
