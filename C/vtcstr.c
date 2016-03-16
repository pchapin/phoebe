/****************************************************************************
FILE          : vtcstr.c
LAST REVISION : May 24, 2002
SUBJECT       : Implementation of the vtc_string abstract type.
PROGRAMMER    : (C) Copyright 2002 by Peter Chapin

The implementation of VTC strings is fairly standard. More memory is
typically allocated for a VTC string than the string actually needs.
When the extra memory is consumed, the total allocation is doubled. This
approach yields amortized constant time for the basic append operation.

VTC strings are currently not reference counted. Each string is
maintained independently of the others. This is easier to implement
correctly, particularly in a multithreaded situation, than reference
counting.

TO DO:

 + Review the formatted copy and append functions. Their handling of
   dynamic memory is not all that it should be. See the comments in the
   functions for more information.

 + Consider switching to an unsigned type for string lengths and
   indicies. That would double the potential size of a string (but what
   about error return values in the search functions?). Also consider
   using a "long" type rather than an "int" type for lengths and
   indicies (more suitable for LP64 architectures).

 + Review the code for thread safety. Document findings (or fix unsafe
   parts).

 + Implement the other four relational operators.

 + Consider changing to a reference counted implementation. Watch thread
   safety issues. (Probably should document performance characteristics
   first to be sure such a change is worthwhile)

 + Write a decent test program.

 + Consider making the code more robust by including run-time checks for
   out of bounds access. Since this abstract type is to be used in a
   classroom situation, agressive error checking would probably be
   desirable. It might even be desirable in real life, too.

 + Rewrite vtc_string_findchar() using a memory search function.

 + Consider implementing a more sophisticated string search algorithm in
   vtc_string_findstring().

 + Considering implementing some additional operations (Reverse string
   searches? Character replacement?)

LICENSE:

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

CONTACT INFO:

Please send comments or bug reports to

     Peter C. Chapin
     P.O. Box 317
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "vtcstr.h"

// Small enough to be easy on memory. Large enough to be useful.
#define INITIAL_CAPACITY 8

//-----------------------------
//      Internal Functions
//-----------------------------

//
// The following function returns the smallest power of 2 that is
// greater than new_size (minimum of INITIAL_CAPACITY). This function is
// used to compute an appropriate new capacity in cases where that is
// necessary. This function makes no provision for overflow.
//
static int round_up(int new_size)
{
  int size = INITIAL_CAPACITY;

  while (size < new_size) size *= 2;
  return size;
}

//-----------------------------
//      External Functions
//-----------------------------

int vtc_string_init(vtc_string *object)
{
  char *temp;

  object->start = NULL;

  temp = malloc(INITIAL_CAPACITY);
  if (temp == NULL) return 0;

  object->start    = temp;
  object->size     = 0;
  object->capacity = INITIAL_CAPACITY;
  return 1;
}


void vtc_string_destroy(vtc_string *object)
{
  free(object->start);
}


int vtc_string_erase(vtc_string *object)
{
  char *temp = realloc(object->start, INITIAL_CAPACITY);
  if (temp == NULL) return 0;

  object->start    = temp;
  object->size     = 0;
  object->capacity = INITIAL_CAPACITY;
  return 1;
}


int vtc_string_copy(vtc_string *object, const vtc_string *other)
{
  int   new_capacity;
  char *temp;

  // If I'm big enough to hold the other, just do the copy. Notice that
  // I make no attempt to reduce my capacity even when the incoming
  // string is considerably shorter than my current string.
  //
  if (other->size <= object->capacity) {
    memcpy(object->start, other->start, other->size);
    object->size = other->size;
  }

  // Otherwise, reallocate myself.
  else {
    new_capacity = round_up(other->size);
    temp = realloc(object->start, new_capacity);
    if (temp == NULL) return 0;
    object->start = temp;
    memcpy(object->start, other->start, other->size);
    object->size = other->size;
    object->capacity = new_capacity;
  }
  return 1;
}


int vtc_string_copycharp(vtc_string *object, const char *other)
{
  int   other_size = strlen(other);
  int   new_capacity;
  char *temp;

  // If I'm big enough to hold the other, just do the copy. Notice that
  // I make no attempt to reduce my capacity even when the incoming
  // string is considerably shorter than my current string.
  //
  if (other_size <= object->capacity) {
    memcpy(object->start, other, other_size);
    object->size = other_size;
  }

  // Otherwise, reallocate myself.
  else {
    new_capacity = round_up(other_size);
    temp = realloc(object->start, new_capacity);
    if (temp == NULL) return 0;
    object->start = temp;
    memcpy(object->start, other, other_size);
    object->size = other_size;
    object->capacity = new_capacity;
  }
  return 1;
}


int vtc_string_copychar(vtc_string *object, char other)
{
  // This version makes no attempt to reduce capacity.
 *object->start = other;
  object->size  = 1;
  return 1;
}


int vtc_string_copyf(vtc_string *object, const char *format, ...)
{
  // This is nasty and hacked. A better version would resize the buffer
  // until it worked and it would then install the buffer directly into
  // the target object (no need to copy it). Something to fix later...

  int     return_value;
  va_list args;
  char   *buffer = (char *)malloc(1024);

  if (buffer == NULL) return 0;

  va_start(args, format);
  vsnprintf(buffer, 1024, format, args);
  return_value = vtc_string_copycharp(object, buffer);
  free(buffer);
  va_end(args);

  return return_value;
}


int vtc_string_append(vtc_string *object, const vtc_string *other)
{
  int   new_capacity;
  char *temp;

  // If the capacity is sufficient, just append.
  if (object->size + other->size <= object->capacity) {
    memcpy(object->start + object->size, other->start, other->size);
    object->size += other->size;
  }

  // Otherwise, reallocate myself.
  else {
    new_capacity = round_up(object->size + other->size);
    temp = realloc(object->start, new_capacity);
    if (temp == NULL) return 0;
    object->start = temp;
    memcpy(object->start + object->size, other->start, other->size);
    object->size += other->size;
    object->capacity = new_capacity;
  }
  return 1;
}


int vtc_string_appendcharp(vtc_string *object, const char *other)
{
  int   other_size = strlen(other);
  int   new_capacity;
  char *temp;

  // If the capacity is sufficient, just append.
  if (object->size + other_size <= object->capacity) {
    memcpy(object->start + object->size, other, other_size);
    object->size += other_size;
  }

  // Otherwise, reallocate myself.
  else {
    new_capacity = round_up(object->size + other_size);
    temp = realloc(object->start, new_capacity);
    if (temp == NULL) return 0;
    object->start = temp;
    memcpy(object->start + object->size, other, other_size);
    object->size += other_size;
    object->capacity = new_capacity;
  }
  return 1;
}


int vtc_string_appendchar(vtc_string *object, char other)
{
  int   new_capacity;
  char *temp;

  // If the capacity is sufficient, just append.
  if (object->size + 1 <= object->capacity) {
    *(object->start + object->size) = other;
    object->size++;
  }

  // Otherwise reallocate myself.
  else {
    new_capacity = round_up(object->size + 1);
    temp = realloc(object->start, new_capacity);
    if (temp == NULL) return 0;
    object->start = temp;
    *(object->start + object->size) = other;
    object->size++;
    object->capacity = new_capacity;
  }
  return 1;
}


int vtc_string_appendf(vtc_string *object, const char *format, ...)
{
  // This is nasty and hacked. A better version would resize the buffer
  // until it worked. It would be nice if we could also avoid the final
  // copy of the buffer into the target object.

  int     return_value;
  va_list args;
  char   *buffer = (char *)malloc(1024);

  if (buffer == NULL) return 0;

  va_start(args, format);
  vsnprintf(buffer, 1024, format, args);
  return_value = vtc_string_appendcharp(object, buffer);
  free(buffer);
  va_end(args);

  return return_value;
}


int vtc_string_prepend(vtc_string *object, const vtc_string *other)
{
  vtc_string temp;

  // Is there a more elegant way to do this?
  if (!vtc_string_init(&temp)        ||
      !vtc_string_copy(&temp, other) ||
      !vtc_string_append(&temp, object)) {

    vtc_string_destroy(&temp);
    return 0;
  }

  // I was able to create temp correctly... now gut it.
  free(object->start);
  object->start    = temp.start;
  object->size     = temp.size;
  object->capacity = temp.capacity;
  return 1;
}


int vtc_string_prependcharp(vtc_string *object, const char *other)
{
  vtc_string temp;

  // Is there a more elegant way to do this?
  if (!vtc_string_init(&temp)             ||
      !vtc_string_copycharp(&temp, other) ||
      !vtc_string_append(&temp, object)) {

    vtc_string_destroy(&temp);
    return 0;
  }

  // I was able to create temp correctly... now gut it.
  free(object->start);
  object->start    = temp.start;
  object->size     = temp.size;
  object->capacity = temp.capacity;
  return 1;
}


int vtc_string_prependchar(vtc_string *object, char other)
{
  vtc_string temp;

  // Is there a more elegant way to do this?
  if (!vtc_string_init(&temp)            ||
      !vtc_string_copychar(&temp, other) ||
      !vtc_string_append(&temp, object)) {

    vtc_string_destroy(&temp);
    return 0;
  }

  // I was able to create temp correctly... now gut it.
  free(object->start);
  object->start    = temp.start;
  object->size     = temp.size;
  object->capacity = temp.capacity;
  return 1;
}


int vtc_string_length(const vtc_string *object)
{
  return object->size;
}


char vtc_string_getcharat(const vtc_string *object, int char_index)
{
  return *(object->start + char_index);
}


char *vtc_string_getcharp(vtc_string *object)
{
  // This method worries me. Will the caller bother to check for NULL?
  // Does it make sense to depend on a byte that is technically outside
  // the bounds of the string?

  if (!vtc_string_appendchar(object, '\0')) return NULL;
  object->size--;
  return object->start;
}


void vtc_string_putcharat(vtc_string *object, char other, int char_index)
{
  *(object->start + char_index) = other;
}


int vtc_string_equal(const vtc_string *left, const vtc_string *right)
{
  if (left->size != right->size) return 0;
  if (memcmp(left->start, right->start, left->size) != 0) return 0;
  return 1;
}

int vtc_string_less(const vtc_string *left, const vtc_string *right)
{
  char *leftp  = left->start;
  char *rightp = right->start;
  int   index;
  
  for (index = 0; index < left->size; index++) {
    if (index >= right->size) break;
    if (*leftp < *rightp) return 1;
    if (*leftp > *rightp) return 0;
    leftp++; rightp++;
  }
  return 0;
}


int vtc_string_findchar(const vtc_string *haystack, char needle)
{
  char *haystackp = haystack->start;
  int   index;

  // Rewrite in terms of a memory search function. (Can't use strchr()
  // because strings might contain embedded null bytes).
  // 
  for (index = 0; index < haystack->size; index++) {
    if (*haystackp == needle) return index;
  }
  return -1;
}


// Is there any value in replacing this with a more sophisticated string
// search algorithm?
//
int vtc_string_findstring(const vtc_string *haystack, const vtc_string *needle)
{
  char *haystackp = haystack->start;
  int   index1;
  int   index2;

  // An empty needle is an error. (Is this right?)
  if (needle->size == 0) return 0;

  for (index1 = 0; index1 < (haystack->size - needle->size + 1); index1++) {
    char *needlep = needle->start;

    for (index2 = index1; index2 < index1 + needle->size; index2++) {
      if (haystackp[index2] != *needlep) break;
    }

    // Did the inner loop end naturally? If so, there is a match.
    if (index2 >= index1 + needle->size) return index1;
  }
  return -1;
}


int vtc_string_substring(
  const vtc_string *source, vtc_string *target, int index, int length)
{
  int temp_index;

  vtc_string_erase(target);
  for (temp_index = index; temp_index < index + length; temp_index++) {
    if (temp_index >= source->size) break;
    if (!vtc_string_appendchar(target, source->start[temp_index])) return 0;
  }
  return 1;
}


int vtc_string_read(vtc_string *object, FILE *infile)
{
  int ch;

  vtc_string_erase(object);

  // Skip leading white space.
  while ((ch = getc(infile)) != EOF) {
    if (ch != ' ' && ch != '\t' && ch != '\f' && ch != '\n') {
      ungetc(ch, infile);
      break;
    }
  }

  // Read the next "word"
  while ((ch = getc(infile)) != EOF) {
    if (ch == ' ' || ch == '\t' || ch == '\f' || ch == '\n') {
      ungetc(ch, infile);
      break;
    }
    if (!vtc_string_appendchar(object, ch)) {
      ungetc(ch, infile);
      return 0;
    }
  }

  return (ch == EOF && object->size == 0) ? 0 : 1;
}


int vtc_string_readline(vtc_string *object, FILE *infile)
{
  int ch;

  vtc_string_erase(object);
  while ((ch = getc(infile)) != EOF) {
    if (ch == '\n') break;
    if (!vtc_string_appendchar(object, ch)) {
      ungetc(ch, infile);
      return 0;
    }
  }

  return (ch == EOF && object->size == 0) ? 0 : 1;
}


int vtc_string_write(const vtc_string *object, FILE *outfile)
{
  char *objectp = object->start;
  int   index;

  for (index = 0; index < object->size; index++) {
    if (fputc(*objectp, outfile) == EOF) return 0;
    objectp++;
  }

  return 1;
}


int vtc_string_writeline(const vtc_string *object, FILE *outfile)
{
  if (!vtc_string_write(object, outfile)) return 0;
  if (fputc('\n', outfile) == EOF) return 0;
  return 1;
}
