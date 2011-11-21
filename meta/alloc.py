#!/usr/bin/python

import sys

objs = {}

while True:
	line = sys.stdin.readline().strip()
	if not line:
		break;
	lines = line.split()
	if lines[0] == "[alloc]":
		# We've got an alloc command
		try:
			foo = objs[lines[1]]
		except KeyError as e:
			objs[lines[1]] = {}
			objs[lines[1]]['type'] = lines[3]
		objs[lines[1]][lines[2]] = True

totalCount  = 0
normalCount = 0
errorCount  = 0
leakCount   = 0

for obj in objs:
	hist = objs[obj]
	try:
		constr = hist['C']
	except KeyError:
		constr = False
	try:
		destr  = hist['D']
	except KeyError:
		destr  = False

	totalCount += 1
	if constr and destr:
		normalCount += 1
	elif constr:
		leakCount += 1
	else:
		errorCount += 1

print "Afer parsing the runlog, here are the leak stats:"
print ""
print "Exactly %s ShibuyaObjects were created." % totalCount
print "** %s ** had an abnormal cycle" % leakCount

if leakCount > 0:
	print ""
	print " **** THERE ARE FIXABLE LEAKS. PLEASE TRACK THEM DOWN ***"
