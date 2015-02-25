#include "tones.h"

typedef struct Song{
	uint16_t length;
	uint8_t note[];
	uint8_t bpm;
} Song;

Song LISA = { 18, {1, G4, 1, A5, 1, B5, 1, C5, 2, D5, 2, D5, 4, E5, 4, F5, 4, E5}, 144 }