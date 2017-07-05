# Overview

My submission uses python and C++, in a sequential method. I did this to use C++ where it is strong, and Python where it is convenient and hopefully 'good enough' in terms of performance. 
Python was used to parse the json files and puts them in a format which can be read easily (didn't want to spend time working on a parser for "{","}" characters, etc. in C++).  C++ was used to form the social network and do heavier lifting. 


Sequence:
1. Python processes both batch and stream .json files into .purchase, .befriend, .unfriend files.
 
2. The C++ code processes the batch_log.* files, meaning the social network and purchase history are processed into memory. 

3. The social network is then updated by the stream_log.befriend/.unfriend to be the most current state. Special care was taken to keep the social network current to the most recent time when analyzing any purchase, even if the social network was slightly different at the time of purchase. (I exercised this because I felt it was more practially relevant.)

4. Then purchases are read in from the stream_log and compared with the batch_log snapshot of purchases. 

5. Any purchases from the stream which should be flagged are written to an intermediate file, which is processed again by Python to the final format. 

6. Finally, the purchase history from the streaming is updated in the entire network. 
This update isn't used anywhere but I just coded it to show how it can be done. 





### Example

