# Antitampering check of ID documents based on the Machine Readable Zone (MRZ)

This *C++* application performs antitampering verification on ID documents by analyzing the Machine Readable Zone (MRZ). The application incorporates *C++* libraries, organizing them into two services: *ocr_server* and *antitampering*.

Within *ocr_server*, two microservices are implemented:
- *ocr*: Utilizes custom Optical Character Recognition (OCR) models trained in *Python* using *Tensorflow*, *Keras*, and *Ultralytics* libraries. Specifically, these models include RetinaNet, RetinaNet-Binary, YOLOv8, and YOLOv8-Binary. The code for training these models can be found at [ID-Documents-OCRs](https://github.com/FrancescaPietrobon/ID-Documents-OCRs).
- *charactersClustering*: Clusters characters extracted by the *ocr* microservice to compute the fields.

On the other hand, the *antitampering*  service solely contains the developed antitampering check for the *mrz*.

To utilize the application, it is imperative to train the models and save them as pb files for the RetinaNet cases and as ONNX for the YOLOv8 cases. Ensure that the trained models are stored in the *models* folder.

Once you have saved the OCR models in the *models* folder, follow these steps to build and execute the application:

To build the application type:
```
conan install . --build missing
conan build .
```

Conan will generate the executables for the application and tests in the *build\Release* folder. Move into this folder with:
```
cd build/Release
```

Next, to extract predictions for the specified image using OCR, Character Clustering, and the association models selected in *src\main*, run:
```
./application
```
    
And to run unit tests execute:
```
./TestOcr
```
