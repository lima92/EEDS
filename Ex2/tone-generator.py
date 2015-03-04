import math

def generate_tones():
	#Number of tones
	tones = [0]*70

	#Start tone (must be an octave of A)
	A1 = 55
	tones[0] = A1

	interrupts_per_sec = 100000

	#12th root of 2
	tone_increment = 1.05946309436

	for i in range(len(tones)-1):
		tones[i + 1] = tones[i]*tone_increment

	for i in range(len(tones)):
		tones[i] = int(interrupts_per_sec / tones[i])

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


	print len("1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, C5, 8, A4, 1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5,1, C5, 1, B5, 1, C5, 8, A4, 3, D4, 1, D4, 6, E4, 2, E4, 2, C5, 2, B5, 2, A4, 2, G4, 1, G4, 1, A4, 1, B5, 2, A4, 2, E4, 4, GF4, 3, D4, 1, D4, 6, E4, 2, E4, 2, C5, 2, B5, 2, A4, 2, G4,6, D5, 2, A4, 4, A4, 3, D4, 1, D4, 6, E4, 2, C5, 2, B5, 2, A4, 2, G4, 2, G4, 1, A4, 1, B5, 2, A4, 2, E4, 4, GF4, 3, D5, 1, D5, 3, G5, 1, F5, 3, EF5, 1, D5, 3, C5, 1, B5, 3, A4, 1, G4,4, D5, 1, A4, 1, A4, 1, A4, 4, A4, 1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5,1, C5, 1, B5, 1, C5, 8, A4, 1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5,1, C5, 1, B5, 1, C5, 8, A4, 1, D5, 1, D5, 1, D5, 12, G5, 1, G4, 1, G4, 1, G4, 12, G4".split(','))


main()