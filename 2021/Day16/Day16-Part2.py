ll = [x for x in open('input.txt').read().strip().split('\n')]

parse = {
	'0':'0000',
	'1':'0001',
 	'2':'0010',
	'3':'0011',
	'4':'0100',
	'5':'0101',
	'6':'0110',
	'7':'0111',
	'8':'1000',
	'9':'1001',
	'A':'1010',
	'B':'1011',
	'C':'1100',
	'D':'1101',
	'E':'1110',
	'F':'1111'
	}

data = ll[0]
data = "".join([parse[x] for x in data])

def parse(data):
    data = data[3:]
    tID = int(data[:3],2)
    data = data[3:]
    if tID == 4:
        temp = ""
        while True:
            temp += data[1:5]
            count = data[0]
            data = data[5:]
            if count == '0':
                break
        return (data, int(temp, 2))

    ltID = data[0]
    data = data[1:]
    subpackv = []
    if ltID == '0':
        l = data[:15]
        data = data[15:]
        subpacketslen = int(l, 2)
        subpackets = data[:subpacketslen]
        while subpackets:
            s,x = parse(subpackets)
            subpackets = s
            subpackv.append(x)
        data = data[subpacketslen:]
    else:
        l = data[:11]
        data = data[11:]
        for _ in range(int(l, 2)):
            s, x = parse(data)
            data = s
            subpackv.append(x)

    if tID == 0:
        return (data, sum(subpackv))
    elif tID == 1:
        p = 1
        for x in subpackv:
            p *= x
        return (data,p)
    elif tID == 2:
        return (data, min(subpackv))
    elif tID == 3:
        return (data, max(subpackv))
    elif tID == 5:
        return (data, 1 if subpackv[0] > subpackv[1] else 0)
    elif tID == 6:
        return (data, 1 if subpackv[0] < subpackv[1] else 0)
    elif tID == 7:
        return (data, 1 if subpackv[0] == subpackv[1] else 0)

print(parse(data)[1])