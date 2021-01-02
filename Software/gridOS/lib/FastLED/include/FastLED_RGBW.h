/* FastLED_RGBW
 * 
 * Hack to enable SK6812 RGBW strips to work with FastLED.
 *
 * Original code by Jim Bumgardner (http://krazydad.com).
 * Modified by David Madison (http://partsnotincluded.com).
 * 
*/

#ifndef FastLED_RGBW_h
#define FastLED_RGBW_h

struct CRGBW  {
	union {
		struct {
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
			union {
				uint8_t w;
				uint8_t white;
			};
		};
		uint8_t raw[4];
		uint32_t u32raw;
	};

	CRGBW(){}

	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
		r = rd;
		g = grn;
		b = blu;
		w = wht;
	}

	CRGBW(CRGB c){
		r = c.r;
		g = c.g;
		b = c.b;
		w = 0;
	}

	inline void operator = (const CRGB c) __attribute__((always_inline)){ 
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->white = 0;
	}

	inline bool operator==(const CRGBW& c)
	{
	return this->r == c.r && this->g == c.g && this->b == c.b && this->w == c.w;
	}
	inline bool operator!=(const CRGBW& c)
	{
	return this->r != c.r || this->g != c.g || this->b != c.b || this->w != c.w;
	}

	inline bool incrementUntil(const CRGBW& desiredColor)
	{
		bool done = true;
		if(this->r < desiredColor.r)
		{
			this->r++;
			done = false;
		}
		if(this->g < desiredColor.g)
		{
			this->g++;
			done = false;
		}
		if(this->b < desiredColor.b)
		{
			this->b++;
			done = false;
		}
		if(this->w < desiredColor.w)
		{
			this->w++;
			done = false;
		}
		return done;
	}
	inline bool decrementUntil(const CRGBW& desiredColor)
	{
		bool done = true;
		if(this->r > desiredColor.r)
		{
			this->r--;
			done = false;
		}
		if(this->g > desiredColor.g)
		{
			this->g--;
			done = false;
		}
		if(this->b > desiredColor.b)
		{
			this->b--;
			done = false;
		}
		if(this->w > desiredColor.w)
		{
			this->w--;
			done = false;
		}
		return done;
	}
};

inline uint16_t getRGBWsize(uint16_t nleds){
	uint16_t nbytes = nleds * 4;
	if(nbytes % 3 > 0) return nbytes / 3 + 1;
	else return nbytes / 3;
}

#endif