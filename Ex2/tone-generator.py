import math

def generate_tones():
	#Number of tones
	tones = [0]*70

	#Start tone (must be an octave of A)
	A1 = 55
	tones[0] = A1

	#12th root of 2
	tone_increment = 1.05946309436

	for i in range(len(tones)-1):
		tones[i + 1] = tones[i]*tone_increment

	for i in range(len(tones)):
		tones[i] = int(100000 / tones[i])

	return tones


def generate_c_code(tones):

	tone_name_counter = 0
	octave = 1
	tone_names = ['A', 'BF', 'B', 'C', 'DF', 'D', 'EF', 'E', 'F', 'GF', 'G', 'AF']
	for i in range(len(tones)):
		
		if len(tone_names) == tone_name_counter:
			tone_name_counter = 0
		if tone_names[tone_name_counter] == 'B':
			octave += 1
		tones[i] = "#define " + str(tone_names[tone_name_counter]) + str(octave) + " " + str(tones[i])
		tone_name_counter += 1
		
	return tones
		

def main():
	a = generate_tones()
	a = generate_c_code(a)
	for line in a:
		print line

main()