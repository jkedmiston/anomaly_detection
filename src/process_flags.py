"""
this script reads an output from C++ and
rewrites it into a json file format
"""
import datetime
import os
fname = "flagged_purchases.flagged"


assert(os.path.isfile(fname))
f = open(fname, "r")
lines = f.readlines()
f.close()
keyorder = ["event_type",
            "timestamp",
            "id",
            "amount",
            "mean",
            "sd"]

g = open("../log_output/flagged_purchases.json", "w")
for kk, line in enumerate(lines):
    time, date, nid, mean, sd, amount = line.split()
    out = datetime.datetime.strptime("%s %s" % (time,  date), "%H:%M:%S %d/%m/%Y")
    json_dict = dict(event_type = "purchase",
                     timestamp = out.strftime("%Y-%m-%d %H:%M:%S"),
                     id = nid,
                     amount = "%2.2f" % float(amount), 
                     mean = "%2.2f" % float(mean),
                     sd = "%2.2f" % float(sd))
    #g.write("{")
    dstring = "{"
    for key in keyorder:
        dstring += '"%s":"%s", ' % (key, json_dict[key])
        pass
    dstring = dstring[0:len(dstring) - 2]
    g.write("%s}\n" % dstring);
    #g.write("%s\n" % (str(json_dict)))
    pass
g.write('\n')
g.close()
print "wrote", os.path.abspath(g.name)
