/****************************************************************************
FILE          : vtcstr.h
LAST REVISION : May 24, 2002
SUBJECT       : Definition of the vtc_string abstract type.
PROGRAMMER    : (C) Copyright 2002 by Peter Chapin

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

/*! \file vtcstr.h
    \brief Declarations of the VTC string methods.
    \author Peter C. Chapin <pchapin@ecet.vtc.edu>
    \date May 24, 2002
    \warning This code has not been formally tested!

    The abstract type vtc_string is a C implementation of a dynamic
    string type. If you use the methods defined here to manipulate VTC
    string objects, VTC strings will take care of their own memory
    management, growing and sometimes shrinking as necessary to hold
    whatever data is put into them.

    VTC strings have value semantics. When a copy of a VTC string is
    made, the new VTC string is independent of the original. VTC strings
    can hold any data, including embedded null characters. VTC strings
    without embedded nulls can be converted into a traditional C-style
    pointer to char. The string can even be modified after the
    conversion provided that no attempt is made to change its size.

    The methods of VTC string mostly return a value of true (non-zero)
    if they are successful. If they fail, for example due to a lack of
    memory, they return false. In that case they leave target objects
    unmodified unless otherwise stated. Some methods assign other
    meanings to their return values. Those meanings are explicitly
    documented with the method.

    String lengths and indicies are represented using the type int.
    Attempts to access a VTC string using an out of range index causes
    undefined behavior. A future version of VTC string may support some
    form of bounds checking, perhaps as a compile time option.

    This code was originally created to support certain classes that I
    teach at Vermont Technical College (hence the "vtc" in the names of
    these functions). However, I invite anyone to use and enhance this
    code. A list of suggested enhancements can be found at the top of
    vtcstr.c.
*/

#ifndef VTCSTR_H
#define VTCSTR_H

#include <stdio.h>

typedef struct {
  // This is a suggested implementation. Other possibilities exist.

  char *start;      //!< Start of string allocation.
  int   size;       //!< Number of characters in string.
  int   capacity;   //!< Number of bytes of reserved space.
} vtc_string;


#ifdef __cplusplus
extern "C" {
#endif

//! Create an empty string
/*!
    \param object Pointer to the VTC string to be initialized.

    \return Zero if the string failed to initialize due to a lack of
    memory resources; non-zero if the initialization was successful.

    In general you should not attempt to use a VTC string that has not
    initialized successfully. However, as an exception, it is safe to
    call vtc_string_destroy() on such a string. If the string failed to
    initialize properly such a call has no effect. Note that it is still
    undefined to call vtc_string_destroy() on a string that has never
    had vtc_string_init() called on it.
*/
int vtc_string_init(vtc_string *object);


//! Release the resources associated with a VTC string.
/*!
    \param object Pointer to the VTC string to be destroyed.

    Do not attempt to use a VTC string after destruction. However, as an
    exception you can reinitialize it with vtc_string_init(). Note that
    it is not an error to call vtc_string_destroy() on a string for
    which vtc_string_init() failed. However, if vtc_string_init() fails,
    calling vtc_string_destroy() is optional.

    This method recovers the memory resources used by a VTC string.
*/
void vtc_string_destroy(vtc_string *object);


//! Make the target VTC string into an empty string.
/*!
    \param object Points to the VTC string to be erased.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the target string is not changed.

    This method removes the contents of the target VTC string. The
    string is left in a usable state. Note that it is highly unlikely
    for this method to fail.
*/
int vtc_string_erase(vtc_string *object);


//! Copy VTC strings.
/*!
    \param object Destination string. Overwritten by the copy.
    \param other Source string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.
*/
int vtc_string_copy(vtc_string *object, const vtc_string *other);


//! Copies a null terminated C style string into a VTC string.
/*!
    \param object Destination string. Overwritten by the copy.
    \param other Pointer to a null terminated C style string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.
*/
int vtc_string_copycharp(vtc_string *object, const char *other);


//! Copy a character into a VTC string.
/*!
    \param object Destination string. Overwritten by the copy.
    \param other Character to copy.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.
*/
int vtc_string_copychar(vtc_string *object, char other);


//! Copy a formatted string into a VTC string.
/*!
    \param object Destination string. Overwritten by the copy.
    \param format printf() style format string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    The format string can contain format specifiers in the style of the
    standard printf() function. Additional parameters must be provided
    as required by the format string. The result of the formatting
    operation is placed into the destination string.
*/
int vtc_string_copyf(vtc_string *object, const char *format, ...);


//! Append one VTC string onto another.
/*!
    \param object Destination string.
    \param other Source string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    A copy of the source string is appended to the end of the
    destination string. 
*/
int vtc_string_append(vtc_string *object, const vtc_string *other);


//! Append a null terminated C style string onto the end of a VTC string.
/*!
    \param object Destination string.
    \param other Points at a null terminated C style string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    A copy of the source string is appended to the end of the
    destination string. 
*/
int vtc_string_appendcharp(vtc_string *object, const char *other);


//! Append a character onto the end of a VTC string.
/*!
    \param object Destination string.
    \param other Character to append.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.
*/
int vtc_string_appendchar(vtc_string *object, char other);


//! Append a formatted string onto the end of a VTC string.
/*!
    \param object Destination string.
    \param format Format string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    The format string can contain format specifiers in the style of the
    standard printf() function. Additional parameters must be provided
    as required by the format string. The result of the formatting
    operation is appended onto the end of the destination string.
*/
int vtc_string_appendf(vtc_string *object, const char *format, ...);


//! Prepend one VTC string onto another.
/*!
    \param object Destination string.
    \param other Source string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    A copy of the source string is inserted at the beginning of the
    destination string.
*/
int vtc_string_prepend(vtc_string *object, const vtc_string *other);


//! Prepend a null terminated C style string onto a VTC string.
/*!
    \param object Destination string.
    \param other Points at a null terminated C style string.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.

    A copy of the source string is inserted at the beginning of the
    destination string.
*/
int vtc_string_prependcharp(vtc_string *object, const char *other);


//! Prepend a character onto a VTC string.
/*!
    \param object Destination string.
    \param other Character to prepend.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if the operation was successful. If the
    operation fails, the destination string is left unchanged.
*/
int vtc_string_prependchar(vtc_string *object, char other);


//! Get the length of a VTC string.
/*!
    \param object String to measure.

    \return The length of the string.

    VTC strings can contain embedded null characters. Such characters
    are counted as part of the strings length. There is no terminating
    null.
*/
int vtc_string_length(const vtc_string *object);


//! Look up a character by its index.
/*!
    \param object String to access.
    \param char_index Index to desired character.

    \return Character at the specified index.

    Attempting to access a character position off the end of the string
    results in undefined behavior.
*/
char vtc_string_getcharat(const vtc_string *object, int char_index);


//! Return a pointer to a null terminated C style string.
/*!
    \param object String to access.

    \return Pointer to the VTC string's internal representation.

    The pointer returned will point at a properly null terminated string
    even though the terminating null character is not actually part of
    the VTC string. Modifing the characters in the VTC string's internal
    representation using this pointer will change the VTC string's
    value. The pointer returned by this method will be invalidated by
    any operation that changes the length of the VTC string.
*/
char *vtc_string_getcharp(vtc_string *object);


//! Modify a character in a VTC string.
/*!
    \param object String to access.
    \param other Character to put into the string.
    \param char_index Index into the string where character is to go.

    This method replaces the character in the VTC string at the
    specified index with the specified character. Attempting to access a
    character position off the end of the string results in undefined
    behavior. This method can not be used to change the length of a VTC
    string.
*/
void vtc_string_putcharat(vtc_string *object, char other, int char_index);


//! Compare two VTC strings for equality.
/*!
    \param left First operand.
    \param right Second operand.

    \return true if the two VTC strings are identical; false otherwise.

    The comparison is done in a case insensitive way. Embedded null
    characters, if any, are handled like any other character. They do
    not terminate the comparison.
*/
int vtc_string_equal(const vtc_string *left, const vtc_string *right);


//! Compare two VTC strings for the less than relationship.
/*!
    \param left First operand.
    \param right Second operand.

    \return true if the first operand is less than the second.

    The first character where the two strings are different has a lower
    ASCII code in the string that is less.
*/
int vtc_string_less(const vtc_string *left, const vtc_string *right);


//! Search a VTC string for a particular character.
/*!
    \param haystack String to search.
    \param needle Character to search for.

    \return Index of the first occurance of the needle character or -1
    if the needle character does not occur.
*/
int vtc_string_findchar(const vtc_string *haystack, char needle);

//! Search a VTC string for another string.
/*!
    \param haystack String to search.
    \param needle String to search for.

    \return Index of the first occurance of the needle string or -1 if
    the needle string does not occur.

    The returned index is the index of the first character of the needle
    string as it appears in the haystack string. For example, searching
    for "foo" in "fizzfoo" returns 4.
*/
int vtc_string_findstring(
  const vtc_string *haystack, const vtc_string *needle);


//! Extract a substring from a VTC string.
/*!
    \param source String to access.
    \param target String where substring will be placed.
    \param index Starting index of substring in source.
    \param length Size of substring.

    \return Zero if the operation fails to complete due to a lack of
    memory resources; non-zero if it is successful. If the operation
    fails the target string will contain a partial result (the part of
    the substring that was copied there before the failure).

    If the specified length goes off the end of the source string, the
    last part of the source string will be taken and the resulting
    substring will not be the specified length. If the initial index is
    out of bounds, the behavior is undefined.
*/
int vtc_string_substring(
  const vtc_string *source, vtc_string *target, int index, int length);


//! Read a white space delimited word from an input file.
/*!
    \param object Destination string.
    \param infile Input file.

    \return Zero if the end of file is encountered without any
    characters being loaded into the destination string OR if an error
    occurs due to a lack of memory resources; non-zero if a string was
    successfully read.

    This method returns the next white space delimited word from the
    input file. Leading white space is skipped. If this method returns
    due to an error condition, the destination string will contain the
    characters successfully read before the error occured. All other
    chararacters will remain on the input stream (no characters are
    lost).
*/
int vtc_string_read(vtc_string *object, FILE *infile);


//! Read a line of text from an input file.
/*!
    \param object Destination string.
    \param infile Input file.

    \return Zero if the end of file is encountered without any
    characters being loaded into the destination string OR if an error
    occurs due to a lack of memory resources; non-zero if a string was
    successfully read.

    This method reads an entire line (of arbitrary length) from the
    input file. It consumes the newline character in the input stream
    but it does not store the newline character in the VTC string. If
    this method returns due to an error condition, the destination
    string will contain the characters successfully read before the
    error occured. All other chararacters will remain on the input
    stream.
*/
int vtc_string_readline(vtc_string *object, FILE *infile);


//! Write a VTC string to an output file.
/*!
    \param object String to write.
    \param outfile Output file.

    \return Zero if an error occurs during the write operation;
    non-zero otherwise.
*/
int vtc_string_write(const vtc_string *object, FILE *outfile);


//! Write a VTC string and a newline to an output file.
/*!
    \param object String to write.
    \param outfile Output file.

    \return Zero if an error occurs during the write operation; non-zero
    otherwise.

    This method differs from vtc_string_write() in that it also writes a
    newline character to the output file after it has writen the
    specified string. This method is intended to work with
    vtc_string_readline().
*/
int vtc_string_writeline(const vtc_string *object, FILE *outfile);

#ifdef __cplusplus
}
#endif

#endif
