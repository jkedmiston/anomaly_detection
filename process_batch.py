import datetime
f = open("batch_log.json", "r")
lines = f.readlines()
f.close()

states = dict(purchase = [],
              befriend = [],
              unfriend = [])

params_file_name = "batch_log.params"
purchase_file_name = "batch_log.purchases"
befriend_file_name = "batch_log.befriends"
unfriend_file_name = "batch_log.unfriends"

for kk, line in enumerate(open("batch_log.json")):
    if(kk % 10000 == 0):
        print "read line %d" % kk
    if(kk == 0):
        d_and_t = eval(line)
        network_dimension = d_and_t["D"] #network dimension
        n_transactions = d_and_t["T"] #transactions
        #print d_and_t["D"]
        #print d_and_t["T"]
        pass
    else:
        batch_line_dict = eval(line)
        timestamp = batch_line_dict["timestamp"]
        timeobj = datetime.datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S")
        fmt = "%H %M %S %d %m %Y"
        event_type = batch_line_dict["event_type"]
        if event_type == "purchase":
            purchase_id = batch_line_dict["id"]
            amount      = batch_line_dict["amount"]
            states["purchase"].append([timeobj.strftime(fmt), purchase_id, amount])
        elif event_type == "befriend":
            id1 = batch_line_dict["id1"]
            id2 = batch_line_dict["id2"]
            states["befriend"].append([timeobj.strftime(fmt), id1, id2])
            pass
        elif event_type == "unfriend":
            id1 = batch_line_dict["id1"]
            id2 = batch_line_dict["id2"]
            states["unfriend"].append([timeobj.strftime(fmt), id1, id2])
            pass
        pass
    pass
params_file = open(params_file_name, "w")
params_file.write("%s %s\n" % (network_dimension, n_transactions))
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
