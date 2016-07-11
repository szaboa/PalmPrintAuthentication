# PalmPrintAuthentication
Palmprint based biometric authentication system implemented in C++, OpenCv 3.0.0, Qt (5.6.0).

##Description
The aim of this thesis project is to develop a palm print based authentication system, which receives an image as input. The application should be able to segment the person’s hand from this image, localize the palm area (ROI), do the feature extraction and save them, or compare them to each other.

The system has three main components. The first component is the  preprocessing module, which task’s is to segment the hand, find the key points, and based  on these key points setup a coordinate system and lastly, extract the palm area. The next  component’s responsibility is to extract the features from the previously stated palm area,  and encode them. The third component is the matching module, which compares and gives  similarity rate of two samples. 

The hand segmentation is based on a Skin Color Model algorithm. This is followed  by the key point localization phase. The key points are those points that are located at  beginning of gaps between the fingers, in other words, these are the valley points. With help  of these positions, the ROI extraction could be done in translation-, rotation invariant way.  The first feature extraction method that I’ve implemented is based on a principal line appro- ach. I have used an edge detection algorithm, which is developed for this specific problem,  and provides a set of parameters to tune the system to its best performance. The distance  between two sample can be calculated by using Chamfer matching in this case. The second  feature extraction method uses a two dimensional Gabor filter. The convolution with  this filter results an image that contains the different texture components of the palm. Using  this method, the matching between two features can be computed by the Hamming distance.  At the end of identification process, the system accepts the user if the distance between  the currently extracted feature and any of the stored features is less than a specified threshold  value, otherwise rejects it.

##Results
The system performance can be evaluated with two indeces: FMR  (False Match Rate) and FNMR (False Non Match Rate). These values can be determined  as a function of the previously mentioned distance threshold and can be used to calculate  the overall accuracy. After tuning the parameters to the system’s best performance, I have  achieved 82.53% correct match rate in case of line based method, and 86.26% with the  texture based approach.

##Database
A public [database](http://www.coep.org.in/resources/coeppalmprintdatabase) is used for the experiments, from College of Engineering, Pune. 

##License
This project is licensed under the MIT License - see the [License.md](/License.md) file for details
