import glob
ports = glob.glob('/dev/tty[A-Za-z]*')
print ports