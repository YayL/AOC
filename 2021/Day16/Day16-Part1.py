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

sumofversions = 0

def parse(data):
	global sumofversions
	version = int(data[:3],2)
	sumofversions += version
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
	else:
		ltID = data[0]
		data = data[1:]
		if ltID == '0':
			l = data[:15]
			data = data[15:]
			subpacketslen = int(l, 2)
			subpackets = data[:subpacketslen]
			while subpackets:
				subpackets = parse(subpackets)
			data = data[subpacketslen:]
		else:
			l = data[:11]
			data = data[11:]
			subpackets_temp = int(l, 2)
			for _ in range(subpackets_temp):
				data = parse(data)
	return data

parse(data)
print(sumofversions)