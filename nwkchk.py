
from sys import argv

if __name__ == "__main__":

	if len(argv) != 2:
		print 'usage: python nwkchk.py [nwk file]'

	f = open(argv[1])

	i = 0 # line number

	current_time = -1

	for l in f:
		i += 1

		a = l.strip().split()

		if len(a) != 3:
			print 'line', str(i), 'has', str(len(a)), 'items'

		for b in a:
			if not b.isdigit():
				print 'non-integer at line', str(i)

		x = [int(b) for b in a]

		for y in x:
			if y < 0:
				print 'negative number at line', str(i)

		if i == 1:
			if x[2] != 0:
				print 'the first time stamp is not zero'

		if x[0] == x[1]:
			print 'self-link at line', str(i)

		if x[0] < x[1]:
			c = a[0] + '_' + a[1]
		else:
			c = a[1] + '_' + a[0]

		if current_time != x[2]:
			contacts = [c]
			if current_time > x[2]:
				print 'contact list not sorted at line', str(i)
			current_time = x[2]
		else:
			if c in contacts:
				print 'multiple contact, (' + a[0] + ',' + a[1] + ',' + a[2] + '), at line', str(i)
			else:
				contacts.append(c)
	
	f.close()






