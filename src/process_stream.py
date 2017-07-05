import datetime
import json_ops

purchase_file_name = "stream_log.purchases"
befriend_file_name = "stream_log.befriends"
unfriend_file_name = "stream_log.unfriends"

states = json_ops.process_batch("../log_input/stream_log.json", 0, __file__)

purchase_file = open(purchase_file_name, "w")
for timestring, pid, amount in states["purchase"]:
    
    purchase_file.write("%s %s %s\n" % (timestring, pid, amount))
    pass
purchase_file.close()
print "wrote", purchase_file.name

befriend_file = open(befriend_file_name, "w")
for timestring, id1, id2 in states["befriend"]:
    befriend_file.write("%s %s %s\n" % (timestring, id1, id2))
    pass
befriend_file.close()
print "wrote", befriend_file.name

unfriend_file = open(unfriend_file_name, "w")
for timestring, id1, id2 in states["unfriend"]:
    unfriend_file.write("%s %s %s\n" % (timestring, id1, id2))
    pass
unfriend_file.close()
print "wrote", unfriend_file.name




