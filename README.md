# MRZ_Antitampering

This C++ application is used to detect MRZ fields in PRADO documents based on a model developed in *Python* using the libraries *Tensorflow*, *Keras*, and *OpenCV*.

To build the application type:
```
conan install . --build missing
conan build .
```

Conan generated the executables for the application and for the tests in the folder *build\Release*, so move into this folder with:
```
cd build\Release
```

Then to extract the prediction run:
```
./application
```
    
And to run unit tests execute:
```
./TestOcr
```
