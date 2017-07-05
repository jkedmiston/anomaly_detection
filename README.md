# Overview

My submission uses Python and C++, in a sequential method (run a Python script, then C++, etc). I did this to use C++ where it is strong, and Python where it is convenient and hopefully 'good enough' in terms of performance for this test. 


Python was used to parse the json files and puts them in a format which can be read easily (didn't want to spend time working on a parser for "{","}" characters, etc. in C++).  C++ was used to form the social network connections and do heavier lifting. 


Sequence:
1. Python processes both batch_log and stream_log .json files into .purchase, .befriend, .unfriend text files, e.g. batch_log.purchase, batch_log.befriend, batch_log.unfriend.
 
2. The C++ code processes these simplified text files, meaning the social network and purchase history are processed into memory. 

3. The social network is then updated by the stream_log.befriend/stream_log.unfriend files which represents the most current state. Care was taken here to keep the social network current to the most recent time when analyzing any purchase, even if the social network was slightly different for a purchase which may be slightly in the past. (I exercised this because I felt it was more practially relevant besides being more straightforward to code). 

4. Then purchases are read in from the stream_log.purchase and compared with the batch_log snapshot of purchases. 

5. Any purchases from the stream which should be flagged are written to an intermediate file, which is processed again by Python to the final format. 

6. Finally, the purchase history from the streaming is updated in the entire network. This update isn't used anywhere but I just coded it to show how it can be done in a real application.


### Tests
Some unit tests for low level functions are in src/tests

execute:

cd src/tests

./run_tests.sh



