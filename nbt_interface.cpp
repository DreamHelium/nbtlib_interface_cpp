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

#include "nbt_interface.hpp"
#include <memory>

static void parse_nbt_real(DhNbtInstance& instance, NBT* nbt)
{
    instance.set_original_nbt(nbt);
    instance.set_current_nbt(nbt);
}

static void parse_nbt_tmp(DhNbtInstance& instance, NBT* nbt)
{
    instance.set_temp_original_nbt(nbt);
    instance.set_current_nbt(nbt);
}

static char *dh_strdup(const char *o_str)
{
#if (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L) || _POSIX_C_SOURCE >= 200809L
    return strdup(o_str); // use strdup if provided
#else
    char* str = malloc( (strlen(o_str) + sizeof("") ) * sizeof(char));
    if(str)
    {
        strcpy(str, o_str);
        return str;
    }
    else return NULL;
#endif
}

static NBT* ret_non_filled_nbt()
{
    NBT* new_nbt = (NBT*)malloc(sizeof(NBT));
    memset(new_nbt, 0, sizeof(NBT));
    return new_nbt;
}

DhNbtInstance::DhNbtInstance(const char* filename)
{
    gsize len = 0;
    guint8* content = nullptr;
    GError* err = nullptr;

    if(g_file_get_contents(filename, (char**)&content, &len, &err))
    {
        NBT* nbt = NBT_Parse(content, len);
        g_free(content);
        if(nbt)
        {
            parse_nbt_real(*this, nbt);
        }
        else original_nbt = nullptr;
    }
    else
    {
        g_error_free(err);
        original_nbt = nullptr;
    }
}

DhNbtInstance::DhNbtInstance(NBT* nbt)
{
    parse_nbt_real(*this, nbt);
}

DhNbtInstance::~DhNbtInstance()
{
}

DhNbtInstance::DhNbtInstance(gint8 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    if(!temporary_root) parse_nbt_real(*this, new_nbt);
    else parse_nbt_tmp(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint16 val, const char *key)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Short;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint32 val, const char *key)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Int;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint64 val, const char *key)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Long;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(float val, const char *key)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Float;
    new_nbt->value_d = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(double val, const char *key)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Double;
    new_nbt->value_d = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(const char *val, const char *key)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_String;
    new_nbt->value_a.value = dh_strdup(val);
    new_nbt->value_a.len = strlen(val) + 1;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint8 *val, int len, const char *key)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int8_t);
    int8_t* new_array = (gint8*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint32 *val, int len, const char *key)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int32_t);
    int32_t* new_array = (gint32*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(gint64 *val, int len, const char *key)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int64_t);
    int64_t* new_array = (gint64*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtInstance::DhNbtInstance(const char *key, DhNbtType type)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = (NBT_Tags)(type - 1);
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt_real(*this, new_nbt);
}

DhNbtType DhNbtInstance::get_type()
{
    if(this->current_nbt)
        return (DhNbtType)(this->current_nbt->type + 1);
    else return DH_TYPE_INVALID;
}

bool DhNbtInstance::is_non_null()
{
    return (this->current_nbt != NULL);
}

bool DhNbtInstance::prev()
{
    if(this->is_non_null())
    {
        this->current_nbt = this->current_nbt->prev;
        return true;
    }
    else return false;
}

bool DhNbtInstance::next()
{
    if(is_non_null())
    {
        current_nbt = current_nbt->prev;
        return true;
    }
    else return false;
}

bool DhNbtInstance::is_type(DhNbtType type)
{
    if(!is_non_null())
    {
        if(type == DH_TYPE_INVALID) return true;
        else return false;
    }
    if(current_nbt->type == (type - 1)) return true;
    else return false;
}

bool DhNbtInstance::parent()
{
    if(tree_struct.size() == 1)
        return false;
    else
    {
        int len = tree_struct.size();
        current_nbt = tree_struct[len - 1];
        tree_struct.resize(len - 1);
        return true;
    }
}

bool DhNbtInstance::child()
{
    if(is_non_null() && (is_type(DH_TYPE_Compound) || is_type(DH_TYPE_List)))
    {
        current_nbt = current_nbt->child;
        tree_struct.push_back(current_nbt);
        return true;
    }
    else return false;
}

void DhNbtInstance::goto_root()
{
    current_nbt = get_original_nbt();
    tree_struct.clear();
}

const char* DhNbtInstance::get_key()
{
    return current_nbt->key;
}

bool DhNbtInstance::prepend(DhNbtInstance child)
{
    if(is_non_null() && child.is_non_null())
    {
        if(is_type(DH_TYPE_List) || is_type(DH_TYPE_Compound))
        {
            current_nbt->child = child.get_current_nbt();
            return true;
        }
        else return false;
    }
    else return false;
}

void DhNbtInstance::set_free_only_instance(bool foi)
{
    if(foi)
    {
        original_nbt_storage.reset((NBT*)nullptr, [](NBT*) {});
    }
    else original_nbt_storage.reset(original_nbt, NBT_Free);
}