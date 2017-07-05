import datetime
def process_batch(fname, has_params = 0, caller = None):
    states = dict(purchase = [],
                  befriend = [],
                  unfriend = [])
    for kk, line in enumerate(open(fname)):
        if line.strip()=='':
            continue
        if(kk % 10000 == 0):
            print caller, ": json_ops.py.process_batch(): reading line %d in %s" % (kk, fname)
        if(kk == 0 and has_params):
            d_and_t = eval(line)
            states["network_dimension"] = d_and_t["D"] #network dimension
            states["n_transactions"] = d_and_t["T"] #transactions
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
    return states;
