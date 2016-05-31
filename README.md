# PalmPrintAuthentication
Palmprint based biometric authentication using C++, OpenCv 3.0.0, Qt(5.6.0).

##Description
The algorithm takes a hand image as input and extracts the palm area (ROI). On the extracted ROI performs a feature extraction (which is implemented in two ways, a line-based and a texture-based approach). After we got the biometric feature of the user, it can be saved or matched (1:N) to the existing samples.

##Results
I have achieved approx. 66% correct match rate with the line-based method, and 92% rate with texture-based features (43200 comparision). More information later.

##Database
A public [database](http://www.coep.org.in/resources/coeppalmprintdatabase) is used for the experiments, from College of Engineering. 

##License
This project is licensed under the MIT License - see the [License.md](/License.md) file for details
