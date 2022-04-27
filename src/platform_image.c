const struct {
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel;
  unsigned char pixel_data[60 * 15 * 2 + 1];
} platform = {
  60, 15, 2,
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
};
