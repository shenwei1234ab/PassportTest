//
//  vnUnicode.cpp
//  vnlib
//
//  Created by Wayne on 14-5-11.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUnicode.h"

_vn_begin

ssize_t Unicode::convert(const c8 *src, size_t src_len, c32 *dest) {
	c32 chr = 0;
	int num = 0;
	c32 *ptr = dest;
	while (src_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				*dest++ = chr;
			}
		} else if (!(u & 0x80)) {
			*dest++ = u;
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c8 *src, size_t src_len, c32 *dest, size_t dest_len) {
	c32 chr = 0;
	int num = 0;
	c32 *ptr = dest;
	while (src_len && dest_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				*dest++ = chr;
				--dest_len;
			}
		} else if (!(u & 0x80)) {
			*dest++ = u;
			--dest_len;
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c8 *src, size_t src_len, str32 &dest) {
	c32 chr = 0;
	int num = 0;
	dest.clear();
	while (src_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				dest.append(1, chr);
			}
		} else if (!(u & 0x80)) {
			dest.append(1, u);
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)dest.length();
}

ssize_t Unicode::convert(const c8 *src, size_t src_len, c16 *dest) {
	c32 chr = 0;
	int num = 0;
	c16 *ptr = dest;
	while (src_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				if (chr > 0x10FFFF) {
					return -1;
				} else if (chr > 0xFFFF) {
					*dest++ = (c16)(((chr >> 10) & 0x3FF) | 0xD800);
					*dest++ = (c16)((chr & 0x3FF) | 0xDC00);
				} else {
					*dest++ = (c16)chr;
				}
			}
		} else if (!(u & 0x80)) {
			*dest++ = u;
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c8 *src, size_t src_len, c16 *dest, size_t dest_len) {
	c32 chr = 0;
	int num = 0;
	c16 *ptr = dest;
	while (src_len && dest_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				if (chr > 0x10FFFF) {
					return -1;
				} else if (chr > 0xFFFF) {
					if (dest_len < 2) {
						break;
					}
					*dest++ = (c16)(((chr >> 10) & 0x3FF) | 0xD800);
					*dest++ = (c16)((chr & 0x3FF) | 0xDC00);
					dest_len -= 2;
				} else {
					*dest++ = (c16)chr;
					--dest_len;
				}
			}
		} else if (!(u & 0x80)) {
			*dest++ = u;
			--dest_len;
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c8 *src, size_t src_len, str16 &dest) {
	c32 chr = 0;
	int num = 0;
	dest.clear();
	while (src_len) {
		c8 u = *src++;
		--src_len;
		if (num) {
			chr = (chr << 6) | (u & 0x3F);
			if (!--num) {
				if (chr > 0x10FFFF) {
					return -1;
				} else if (chr > 0xFFFF) {
					dest.append(1, (c16)(((chr >> 10) & 0x3FF) | 0xD800));
					dest.append(1, (c16)((chr & 0x3FF) | 0xDC00));
				} else {
					dest.append(1, (c16)chr);
				}
			}
		} else if (!(u & 0x80)) {
			dest.append(1, u);
		} else if (!(u & 0x40)) {
			return -1;
		} else if (!(u & 0x20)) {
			num = 1;
			chr = u & 0x1F;
		} else if (!(u & 0x10)) {
			num = 2;
			chr = u & 0x0F;
		}  else if (!(u & 0x08)) {
			num = 3;
			chr = u & 0x07;
		} else if (!(u & 0x04)) {
			num = 4;
			chr = u & 0x03;
		} else if (!(u & 0x02)) {
			num = 5;
			chr = u & 0x01;
		} else {
			return -1;
		}
	}
	if (num) {
		return -1;
	}
	return (ssize_t)dest.length();
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, c8 *dest) {
	c8 *ptr = dest;
	bool ext = false;
	c32 chr = 0;
	while (src_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			*dest++ = (c8)chr;
		} else if (chr < 0x800) {
			*dest++ = (c8)(0xC0 | (chr >> 6));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x10000) {
			*dest++ = (c8)(0xE0 | (chr >> 12));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x200000) {
			*dest++ = (c8)(0xF0 | (chr >> 18));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x4000000) {
			*dest++ = (c8)(0xF8 | (chr >> 24));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else {
			*dest++ = (c8)(0xFC | (chr >> 30));
			*dest++ = (c8)(0x80 | ((chr >> 24) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		}
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, c8 *dest, size_t dest_len) {
	c8 *ptr = dest;
	bool ext = false;
	c32 chr = 0;
	while (src_len && dest_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			*dest++ = (c8)chr;
			--dest_len;
		} else if (chr < 0x800) {
			if (dest_len < 2) {
				break;
			}
			*dest++ = (c8)(0xC0 | (chr >> 6));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 2;
		} else if (chr < 0x10000) {
			if (dest_len < 3) {
				break;
			}
			*dest++ = (c8)(0xE0 | (chr >> 12));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 3;
		} else if (chr < 0x200000) {
			if (dest_len < 4) {
				break;
			}
			*dest++ = (c8)(0xF0 | (chr >> 18));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 4;
		} else if (chr < 0x4000000) {
			if (dest_len < 5) {
				break;
			}
			*dest++ = (c8)(0xF8 | (chr >> 24));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 5;
		} else {
			if (dest_len < 6) {
				break;
			}
			*dest++ = (c8)(0xFC | (chr >> 30));
			*dest++ = (c8)(0x80 | ((chr >> 24) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 6;
		}
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, str8 &dest) {
	dest.clear();
	bool ext = false;
	c32 chr = 0;
	while (src_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			dest.append(1, (c8)chr);
		} else if (chr < 0x800) {
			dest.append(1, (c8)(0xC0 | (chr >> 6)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x10000) {
			dest.append(1, (c8)(0xE0 | (chr >> 12)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x200000) {
			dest.append(1, (c8)(0xF0 | (chr >> 18)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x4000000) {
			dest.append(1, (c8)(0xF8 | (chr >> 24)));
			dest.append(1, (c8)(0x80 | ((chr >> 18) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else {
			dest.append(1, (c8)(0xFC | (chr >> 30)));
			dest.append(1, (c8)(0x80 | ((chr >> 24) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 18) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		}
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)dest.length();
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, c32 *dest) {
	c32 *ptr = dest;
	bool ext = false;
	c32 chr = 0;
	while (src_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		*dest++ = chr;
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, c32 *dest, size_t dest_len) {
	c32 *ptr = dest;
	bool ext = false;
	c32 chr = 0;
	while (src_len && dest_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		*dest++ = chr;
		--dest_len;
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c16 *src, size_t src_len, str32 &dest) {
	dest.clear();
	bool ext = false;
	c32 chr = 0;
	while (src_len) {
		const c16 t = *src++;
		--src_len;
		if (!ext) {
			if ((t & 0xFC00) == 0xD800) {
				ext = true;
				chr = t & 0x3FF;
				continue;
			}
			chr = (u16)t;
		} else if ((t & 0xFC00) != 0xDC00)  {
			return -1;
		} else {
			chr = (chr << 10) | (t & 0x3FF);
			ext = false;
		}
		dest.append(1, chr);
	}
	if (ext) {
		return -1;
	}
	return (ssize_t)dest.length();
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, c8 *dest) {
	c8 *ptr = dest;
	while (src_len) {
		const c32 chr = *src++;
		--src_len;
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			*dest++ = (c8)chr;
		} else if (chr < 0x800) {
			*dest++ = (c8)(0xC0 | (chr >> 6));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x10000) {
			*dest++ = (c8)(0xE0 | (chr >> 12));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x200000) {
			*dest++ = (c8)(0xF0 | (chr >> 18));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else if (chr < 0x4000000) {
			*dest++ = (c8)(0xF8 | (chr >> 24));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		} else {
			*dest++ = (c8)(0xFC | (chr >> 30));
			*dest++ = (c8)(0x80 | ((chr >> 24) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
		}
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, c8 *dest, size_t dest_len) {
	c8 *ptr = dest;
	while (src_len && dest_len) {
		const c32 chr = *src++;
		--src_len;
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			*dest++ = (c8)chr;
			--dest_len;
		} else if (chr < 0x800) {
			if (dest_len < 2) {
				break;
			}
			*dest++ = (c8)(0xC0 | (chr >> 6));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 2;
		} else if (chr < 0x10000) {
			if (dest_len < 3) {
				break;
			}
			*dest++ = (c8)(0xE0 | (chr >> 12));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 3;
		} else if (chr < 0x200000) {
			if (dest_len < 4) {
				break;
			}
			*dest++ = (c8)(0xF0 | (chr >> 18));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 4;
		} else if (chr < 0x4000000) {
			if (dest_len < 5) {
				break;
			}
			*dest++ = (c8)(0xF8 | (chr >> 24));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 5;
		} else {
			if (dest_len < 6) {
				break;
			}
			*dest++ = (c8)(0xFC | (chr >> 30));
			*dest++ = (c8)(0x80 | ((chr >> 24) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 18) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 12) & 0x3F));
			*dest++ = (c8)(0x80 | ((chr >> 6) & 0x3F));
			*dest++ = (c8)(0x80 | (chr & 0x3F));
			dest_len -= 6;
		}
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, str8 &dest) {
	dest.clear();
	while (src_len) {
		const c32 chr = *src++;
		--src_len;
		if (chr & 0x80000000) {
			return -1;
		} else if (chr < 0x80) {
			dest.append(1, (c8)chr);
		} else if (chr < 0x800) {
			dest.append(1, (c8)(0xC0 | (chr >> 6)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x10000) {
			dest.append(1, (c8)(0xE0 | (chr >> 12)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x200000) {
			dest.append(1, (c8)(0xF0 | (chr >> 18)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else if (chr < 0x4000000) {
			dest.append(1, (c8)(0xF8 | (chr >> 24)));
			dest.append(1, (c8)(0x80 | ((chr >> 18) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		} else {
			dest.append(1, (c8)(0xFC | (chr >> 30)));
			dest.append(1, (c8)(0x80 | ((chr >> 24) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 18) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 12) & 0x3F)));
			dest.append(1, (c8)(0x80 | ((chr >> 6) & 0x3F)));
			dest.append(1, (c8)(0x80 | (chr & 0x3F)));
		}
	}
	return (ssize_t)dest.length();
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, c16 *dest) {
	c16 *ptr = dest;
	while (src_len) {
		c32 chr = *src++;
		--src_len;
		if (chr > 0x10FFFF) {
			return -1;
		} else if (chr > 0xFFFF) {
			*dest++ = (c16)(((chr >> 10) & 0x3FF) | 0xD800);
			*dest++ = (c16)((chr & 0x3FF) | 0xDC00);
		} else {
			*dest++ = (c16)chr;
		}
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, c16 *dest, size_t dest_len) {
	c16 *ptr = dest;
	while (src_len && dest_len) {
		c32 chr = *src++;
		--src_len;
		if (chr > 0x10FFFF) {
			return -1;
		} else if (chr > 0xFFFF) {
			if (dest_len < 2) {
				break;
			}
			*dest++ = (c16)(((chr >> 10) & 0x3FF) | 0xD800);
			*dest++ = (c16)((chr & 0x3FF) | 0xDC00);
			dest_len -= 2;
		} else {
			*dest++ = (c16)chr;
			--dest_len;
		}
	}
	return (ssize_t)(dest - ptr);
}

ssize_t Unicode::convert(const c32 *src, size_t src_len, str16 &dest) {
	dest.clear();
	while (src_len) {
		c32 chr = *src++;
		--src_len;
		if (chr > 0x10FFFF) {
			return -1;
		} else if (chr > 0xFFFF) {
			dest.append(1, (c16)(((chr >> 10) & 0x3FF) | 0xD800));
			dest.append(1, (c16)((chr & 0x3FF) | 0xDC00));
		} else {
			dest.append(1, (c16)chr);
		}
	}
	return (ssize_t)dest.length();
}

_vn_end
