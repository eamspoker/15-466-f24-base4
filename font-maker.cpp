#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <iostream>
#include "load_save_png.hpp"


void make_font(const char *text,  const int fontSize, const size_t length)
{
	size_t _checks = 0;
	size_t *checks = &_checks;
	auto checkFTerror = [&](FT_Error error){
		if (error)
		{
			throw std::runtime_error("Error " + std::to_string(*checks));
		}
		*checks = *checks + 10;
	};

	FT_Library library;
	FT_Face face;
	FT_Error error = FT_Init_FreeType( &library );
	checkFTerror(error);
	error = FT_New_Face(library, "DejaVuSansMono.ttf", 0, &face);
	checkFTerror(error);

	// adapted from the harfbuzz example
	hb_font_t *hb_font;
	hb_font = hb_ft_font_create (face, NULL);

	hb_buffer_t *buf = hb_buffer_create();

	// add text to buffer
	hb_buffer_add_utf8(buf, text, -1, 0, -1);

	// fill in unset buffer properties
	hb_buffer_guess_segment_properties (buf);

	// shape using font
	hb_shape (hb_font, buf, NULL, 0);
	// size_t pen_x, pen_y = 0;
	for (size_t g = 0; g < length; g++)
	{
		FT_UInt  glyph_index;
		
		glyph_index = FT_Get_Char_Index( face, text[g] );

		error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
		checkFTerror(error);
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

		std::vector<glm::u8vec4> colorvec;

		for (size_t i = 0; i < face->glyph->bitmap.rows; i++)
		{
			for (size_t j = 0; j < face->glyph->bitmap.width; j++)
			{
				glm::u8vec4 color = glm::u8vec4(0);
				color.r = face->glyph->bitmap.buffer[i*face->glyph->bitmap.width + j];
				color.a = 255;
				colorvec.emplace_back(color);
			}
			
		}

		glm::uvec2 size = {face->glyph->bitmap.rows, face->glyph->bitmap.width};

		save_png("letter" + std::to_string(g) + ".png", size, colorvec.data(), UpperLeftOrigin);
	}

	

	// cleanup code from harfbuzz example
	hb_buffer_destroy(buf);
	hb_font_destroy(hb_font);
	FT_Done_Face(face);
	FT_Done_FreeType(library);

  
}

// adapted from freetype-test
int main(int argc, char **argv) {


	  if (argc < 2)
		{
			throw std::runtime_error("Too few arguments.");
		}

		make_font(argv[1], 36, 5);
}
