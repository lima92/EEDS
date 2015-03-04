#include "tones.h"

typedef struct Song{
	uint16_t length;
	uint8_t bpm;
	uint16_t note[];
} Song;

Song LISA = { 20, 144, {1, C4, 1, D4, 1, E4, 1, F4, 2, G4, 2, G4, 1, A4, 1, A4, 1, A4, 1, A4, 4, G4, 1, F4, 1, F4, 1, F4, 1, F4, 2, E4, 2, E4, 1, D4, 1, D4, 1, D4, 1, D4, 4, C4} };

Song STAR_WARS = {34, 72, {1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5,   
						   1, C5, 1, B5, 1, C5, 8, A4, 1, D4, 1, D4, 1, D4, 8, G4, 8, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5, 1, C5, 1, B5, 1, A4, 8, G5, 4, D5,
						   1, C5, 1, B5, 1, C5, 8, A4, 3, D4, 1, D4, 5, E4, 2, E4, 2, C5, 2, B5, 2, A4, 2, G4, 1, G4, 1, A4, 1, B5, 2, A4, 2, E4, 4, F4, 3, D4, 1, D4, 5, E4, 2, E4, 2, C5, 2, B5, 2, A4, 2, G4}};