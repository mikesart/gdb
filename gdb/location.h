/* Data structures and API for event locations in GDB.
   Copyright (C) 2013-2015 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef LOCATIONS_H
#define LOCATIONS_H 1

struct language_defn;
struct event_location;

/* An enumeration of possible signs for a line offset.  */

enum offset_relative_sign
{
  /* No sign  */
  LINE_OFFSET_NONE,

  /* A plus sign ("+")  */
  LINE_OFFSET_PLUS,

  /* A minus sign ("-")  */
  LINE_OFFSET_MINUS,

  /* A special "sign" for unspecified offset.  */
  LINE_OFFSET_UNKNOWN
};

/* A line offset in a location.  */

struct line_offset
{
  /* Line offset and any specified sign.  */
  int offset;
  enum offset_relative_sign sign;
};

/* An enumeration of the various ways to specify a stop event
   location (used with create_breakpoint).  */

enum event_location_type
{
  /* A traditional linespec.  */
  LINESPEC_LOCATION,

  /* An address in the inferior.  */
  ADDRESS_LOCATION,

  /* An explicit location.  */
  EXPLICIT_LOCATION,

  /* A probe location.  */
  PROBE_LOCATION
};

/* An explicit location.  This structure is used to bypass the
   parsing done on linespecs.  It still has the same requirements
   as linespecs, though.  For example, source_filename requires
   at least one other field.  */

struct explicit_location
{
  /* The source filename. Malloc'd.  */
  char *source_filename;

  /* The function name.  Malloc'd.  */
  char *function_name;

  /* The name of a label.  Malloc'd.  */
  char *label_name;

  /* A line offset relative to the start of the symbol
     identified by the above fields or the current symtab
     if the other fields are NULL.  */
  struct line_offset line_offset;
};

/* Return the type of the given event location.  */

extern enum event_location_type
  event_location_type (const struct event_location *);

/* Return a malloc'd explicit string representation of the given
   explicit location.  The location must already be canonicalized/valid.  */

extern char *
  explicit_location_to_string (const struct explicit_location *explicit);

/* Return a malloc'd linespec string representation of the given
   explicit location.  The location must already be canonicalized/valid.  */

extern char *
  explicit_location_to_linespec (const struct explicit_location *explicit);

/* Return a string representation of the LOCATION.
   This function may return NULL for unspecified linespecs,
   e.g, LOCATION_LINESPEC and addr_string is NULL.

   The result is cached in LOCATION.  */

extern const char *
  event_location_to_string (struct event_location *location);

/* Create a new linespec location.  The return result is malloc'd
   and should be freed with delete_event_location.  */

extern struct event_location *
  new_linespec_location (char **linespec);

/* Return the linespec location (a string) of the given event_location
   (which must be of type LINESPEC_LOCATION).  */

extern const char *
  get_linespec_location (const struct event_location *location);

/* Create a new address location.  The return result is malloc'd
   and should be freed with delete_event_location.  */

extern struct event_location *
  new_address_location (CORE_ADDR addr);

/* Return the address location (a CORE_ADDR) of the given event_location
   (which must be of type ADDRESS_LOCATION).  */

extern CORE_ADDR
  get_address_location (const struct event_location *location);

/* Create a new probe location.  The return result is malloc'd
   and should be freed with delete_event_location.  */

extern struct event_location *
  new_probe_location (const char *probe);

/* Return the probe location (a string) of the given event_location
   (which must be of type PROBE_LOCATION).  */

extern const char *
  get_probe_location (const struct event_location *location);

/* Initialize the given explicit location.  */

extern void initialize_explicit_location (struct explicit_location *explicit);

/* Create a new explicit location.  If not NULL, EXPLICIT is checked for
   validity.  If invalid, an exception is thrown.

   The return result is malloc'd and should be freed with
   delete_event_location.  */

extern struct event_location *
  new_explicit_location (const struct explicit_location *explicit);

/* Return the explicit location of the given event_location
   (which must be of type EXPLICIT_LOCATION).  */

extern struct explicit_location *
  get_explicit_location (struct event_location *location);

/* A const version of the above.  */

extern const struct explicit_location *
  get_explicit_location_const (const struct event_location *location);

/* Free an event location and any associated data.  */

extern void delete_event_location (struct event_location *location);

/* Make a cleanup to free LOCATION.  */

extern struct cleanup *
  make_cleanup_delete_event_location (struct event_location *location);

/* Return a copy of the given SRC location.  */

extern struct event_location *
  copy_event_location (const struct event_location *src);

/* Attempt to convert the input string in *ARGP into an event_location.
   ARGP is advanced past any processed input.  Returns an event_location
   (malloc'd) if an event location was successfully found in *ARGP,
   NULL otherwise.

   This function may call error() if *ARGP looks like properly formed,
   but invalid, input, e.g., if it is called with missing argument parameters
   or invalid options.

   The return result must be freed with delete_event_location.  */

extern struct event_location *
  string_to_event_location (char **argp,
			    const struct language_defn *langauge);

/* A convenience function for testing for unset locations.  */

extern int event_location_empty_p (const struct event_location *location);

/* Set the location's string representation.  If STRING is NULL, clear
   the string representation.  */

extern void
  set_event_location_string (struct event_location *location,
			     const char *string);
#endif /* LOCATIONS_H */
