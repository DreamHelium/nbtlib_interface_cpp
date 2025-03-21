/*  nbt_interface - NBT Lib Interface
    Copyright (C) 2025 Dream Helium
    This file is part of nbtlib_interface.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef NBT_INTERFACE_HPP
#define NBT_INTERFACE_HPP

/* The enum type for recognize */

#ifndef DH_NBT_TYPES
#define DH_NBT_TYPES
typedef enum {
    DH_TYPE_INVALID, DH_TYPE_End, DH_TYPE_Byte, DH_TYPE_Short, DH_TYPE_Int, DH_TYPE_Long, DH_TYPE_Float, DH_TYPE_Double, DH_TYPE_Byte_Array, DH_TYPE_String, DH_TYPE_List, DH_TYPE_Compound, DH_TYPE_Int_Array, DH_TYPE_Long_Array} DhNbtType;
#endif

#include "libnbt/nbt.h"
#include <glib.h>
#include <vector>
#include <memory>

class DhNbtInstance
{
public:
  DhNbtInstance(const char *filename);
  [[deprecated("This function will take ownership of the NBT, so please use the filename instead!")]]
  DhNbtInstance(NBT *root);
  ~DhNbtInstance();

  DhNbtInstance(gint8 val, const char *key, bool temporary_root);
  DhNbtInstance(gint16 val, const char *key);
  DhNbtInstance(gint32 val, const char *key);
  DhNbtInstance(gint64 val, const char *key);
  DhNbtInstance(float val, const char *key);
  DhNbtInstance(double val, const char *key);
  DhNbtInstance(const char *val, const char *key);
  DhNbtInstance(gint8 *val, int len, const char *key);
  DhNbtInstance(gint32 *val, int len, const char *key);
  DhNbtInstance(gint64 *val, int len, const char *key);
  DhNbtInstance(const char *key, DhNbtType type);

  void set_free_only_instance(bool foi);

  NBT *get_original_nbt() { return original_nbt; }
  NBT *get_current_nbt() { return current_nbt; }
  auto get_tree_struct() { return tree_struct; }
  int  get_nbt_rc()      { return original_nbt_storage.use_count(); }

  void set_original_nbt(NBT* nbt) 
  { 
    original_nbt = nbt;
    original_nbt_storage.reset(nbt, NBT_Free); 
  }
  void set_temp_original_nbt(NBT* nbt)
  {
    original_nbt = nbt;
    original_nbt_storage.reset(nbt, [](NBT*) {});
  }
  void set_current_nbt(NBT* nbt)  { current_nbt = nbt; }
  void set_tree_struct(std::vector<NBT*> arr) { tree_struct = arr; }

  DhNbtType get_type();
  bool is_non_null();
  bool prev();
  bool next();
  bool parent();
  bool child();
  void goto_root();
  bool is_type(DhNbtType type);
  const char *get_key();

  gint8 get_byte();
  gint16 get_short();
  gint32 get_int();
  gint64 get_long();
  gint64 get_integer();

  /* The array type should not be freed unless the memory is freed! */
  const gchar *get_string();
  const gint8 *get_byte_array();
  const gint32 *get_int_array();
  const gint64 *get_long_array();

  bool prepend(DhNbtInstance child);
  bool insert_after(DhNbtInstance sibling, DhNbtInstance node);
  bool insert_before(DhNbtInstance sibling, DhNbtInstance node);

  bool save_to_file(const char *pos);

private:
    /* Root NBT storage */
    std::shared_ptr<NBT> original_nbt_storage;
    /* Real Root NBT */
    NBT* original_nbt;
    /* The current position of NBT */
    NBT* current_nbt;
    /* This can be nonexist, based on the implement */
    std::vector<NBT*> tree_struct;
};

#endif /* NBT_INTERFACE_HPP */