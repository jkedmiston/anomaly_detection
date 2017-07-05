import datetime
import json_ops
params_file_name = "batch_log.params"
purchase_file_name = "batch_log.purchases"
befriend_file_name = "batch_log.befriends"
unfriend_file_name = "batch_log.unfriends"

states = json_ops.process_batch("../log_input/batch_log.json", has_params = 1, caller = __file__)

params_file = open(params_file_name, "w")
params_file.write("%s %s\n" % (states["network_dimension"], states["n_transactions"]))
params_file.close()

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

