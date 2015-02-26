#include "tones.h"

typedef struct Song{
	uint16_t length;
	uint8_t bpm;
	uint16_t note[];
} Song;

Song LISA = { 20, 144, {1, C4, 1, D4, 1, E4, 1, F4, 2, G4, 2, G4, 1, A4, 1, A4, 1, A4, 1, A4, 4, G4, 1, F4, 1, F4, 1, F4, 1, F4, 2, E4, 2, E4, 1, D4, 1, D4, 1, D4, 1, D4, 4, C4} };
