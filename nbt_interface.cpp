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
#include <stdexcept>
#include <gio/gio.h>

static void parse_nbt_real(DhNbtInstance& instance, NbtNode* nbt)
{
    instance.set_original_nbt(nbt);
    instance.set_current_nbt(nbt);
}

static void parse_nbt_tmp(DhNbtInstance& instance, NbtNode* nbt)
{
    instance.set_temp_original_nbt(nbt);
    instance.set_current_nbt(nbt);
}

static void parse_nbt(DhNbtInstance& instance, NbtNode* nbt, bool tr)
{
    if(tr) parse_nbt_tmp(instance, nbt);
    else   parse_nbt_real(instance, nbt);
}

static bool has_child(DhNbtInstance parent, DhNbtInstance child)
{
    NbtNode* parent_node = parent.get_current_nbt();
    if(parent_node->children && child.is_non_null())
    {
        NbtNode* child_node = parent_node->children;
        for(; child_node ; child_node = child_node->next)
        {
            if(child_node == child.get_current_nbt()) return true;
        }
        return false;
    }
    else if(child.is_non_null()) /* Parent node has no child */
        return false;
    else return true;  /* child is null */
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

static NbtNode* ret_non_filled_nbt()
{
    NbtNode* ret = g_node_new(NULL);
    return ret;
}

DhNbtInstance::DhNbtInstance(const char* filename)
{
    gsize len = 0;
    guint8* content = nullptr;
    GError* err = nullptr;

    if(g_file_get_contents(filename, (char**)&content, &len, &err))
    {
        NbtNode* nbt = nbt_node_new(content, len);
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

DhNbtInstance::DhNbtInstance(const char* filename, bool temporary_root)
{
    gsize len = 0;
    guint8* content = nullptr;
    GError* err = nullptr;

    if(g_file_get_contents(filename, (char**)&content, &len, &err))
    {
        NbtNode* nbt = nbt_node_new(content, len);
        g_free(content);
        if(nbt)
        {
            parse_nbt(*this, nbt, temporary_root);
        }
        else original_nbt = nullptr;
    }
    else
    {
        g_error_free(err);
        original_nbt = nullptr;
    }
}

DhNbtInstance::DhNbtInstance(NbtNode* nbt, bool tr)
{
    parse_nbt(*this, nbt, tr);
}

DhNbtInstance::~DhNbtInstance()
{
}

DhNbtInstance::DhNbtInstance(gint8 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Byte;
    new_nbt_data->value_i = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint16 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Short;
    new_nbt_data->value_i = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint32 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Int;
    new_nbt_data->value_i = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint64 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Long;
    new_nbt_data->value_i = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(float val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Float;
    new_nbt_data->value_d = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(double val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Double;
    new_nbt_data->value_d = val;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const char *val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_String;
    new_nbt_data->value_a.value = dh_strdup(val);
    new_nbt_data->value_a.len = strlen(val) + 1;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const gint8 *val, int len, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Byte_Array;
    int byte = len * sizeof(int8_t);
    int8_t* new_array = (gint8*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt_data->value_a.value = new_array;
    new_nbt_data->value_a.len = len;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const gint32 *val, int len, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Int_Array;
    int byte = len * sizeof(int32_t);
    int32_t* new_array = (gint32*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt_data->value_a.value = new_array;
    new_nbt_data->value_a.len = len;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const gint64 *val, int len, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = TAG_Long_Array;
    int byte = len * sizeof(int64_t);
    int64_t* new_array = (gint64*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt_data->value_a.value = new_array;
    new_nbt_data->value_a.len = len;
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(DhNbtType type, const char* key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    auto new_nbt_data = g_new0(NbtData, 1);
    new_nbt_data->type = (NBT_Tags)(type - 1);
    new_nbt_data->key = key ? dh_strdup(key) : NULL;
    new_nbt->data = new_nbt_data;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtType DhNbtInstance::get_type()
{
    if(this->current_nbt)
        {
            NbtData* data = (NbtData*)this->current_nbt->data;
            return (DhNbtType)(data->type + 1);
        }
    else return DH_TYPE_INVALID;
}

bool DhNbtInstance::is_non_null()
{
    return (this->current_nbt != NULL);
}

bool DhNbtInstance::prev()
{
    if(is_non_null())
    {
        current_nbt = current_nbt->prev;
        return true;
    }
    else return false;
}

bool DhNbtInstance::next()
{
    if(is_non_null())
    {
        current_nbt = current_nbt->next;
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
    auto data = (NbtData*)this->current_nbt->data;
    if(data->type == (type - 1)) return true;
    else return false;
}

bool DhNbtInstance::parent()
{
    if(is_non_null())
        {
            current_nbt = current_nbt->parent;
            return true;
        }
    else return false;
}

bool DhNbtInstance::child()
{
    if(is_non_null())
        {
            current_nbt = current_nbt->children;
            return true;
        }
    else return false;
}

bool DhNbtInstance::child(const char* key)
{
    if(child())
    {
        do
        {
            if(get_key() && !strcmp(get_key(), key))
                return true;
        }
        while(next());
        parent();
        return false;
    }
    else return false;
}

bool DhNbtInstance::child(int index)
{
    if(child())
    {
        bool ret = true;
        for(int i = 0 ; i < index ; i++)
            ret = next();
        if(ret) return true;
        parent();
        return false;
    }
    else return false;
}

void DhNbtInstance::goto_root()
{
    current_nbt = get_original_nbt();
}

const char* DhNbtInstance::get_key()
{
    if(is_non_null())
        {
            NbtData* data = (NbtData*)this->current_nbt->data;
                    return data->key;

        }
    else return nullptr;
}

void DhNbtInstance::set_key(const char* key)
{
    if(is_non_null())
    {
            NbtData* data = (NbtData*)this->current_nbt->data;
        free(data->key);
        data->key = key ? dh_strdup(key) : nullptr;
    }
}

void DhNbtInstance::make_invalid()
{
    current_nbt = nullptr;
}

bool DhNbtInstance::prepend(DhNbtInstance child)
{
    if(is_non_null() && child.is_non_null())
    {
        if(is_type(DH_TYPE_List) || is_type(DH_TYPE_Compound))
        {
            g_node_prepend(current_nbt, child.current_nbt);
            return true;
        }
        else return false;
    }
    else return false;
}

bool DhNbtInstance::insert_after(DhNbtInstance sibling, DhNbtInstance node)
{
    if(has_child(*this, sibling))
    {
        g_node_insert_after (current_nbt, sibling.current_nbt, node.current_nbt);
        return true;
    }
    else return false;
}

bool DhNbtInstance::insert_before(DhNbtInstance sibling, DhNbtInstance node)
{
    if(has_child(*this, sibling))
    {
        g_node_insert_before(current_nbt, sibling.current_nbt, node.current_nbt);
        return true;
    }
    else return false;
}

gint8 DhNbtInstance::get_byte()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Byte)) return data->value_i;
    else throw std::domain_error("Not the right type!");
}

gint16 DhNbtInstance::get_short()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Short)) return data->value_i;
    else throw std::domain_error("Not the right type!");
}

gint32 DhNbtInstance::get_int()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Int)) return data->value_i;
    else throw std::domain_error("Not the right type!");
}

gint64 DhNbtInstance::get_long()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Long)) return data->value_i;
    else throw std::domain_error("Not the right type!");
}

gint64 DhNbtInstance::get_integer()
{
    auto type = get_type();
    auto data = (NbtData*)this->current_nbt->data;
    if(type >= DH_TYPE_Byte && type <= DH_TYPE_Long)
        return data->value_i;
    else throw std::domain_error("Not the right type!");
}

float DhNbtInstance::get_float()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Float)) return data->value_d;
    else throw std::domain_error("Not the right type!");
}

double DhNbtInstance::get_double()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Double)) return data->value_d;
    else throw std::domain_error("Not the right type!");
}

const gchar* DhNbtInstance::get_string()
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_String)) return (const char*)data->value_a.value;
    else throw std::domain_error("Not the right type!");
}

const gint8* DhNbtInstance::get_byte_array(int& len)
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Byte_Array))
    {
        len = data->value_a.len;
        return (gint8*)data->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
}


const gint32* DhNbtInstance::get_int_array(int& len)
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Int_Array))
    {
        len = data->value_a.len;
        return (gint32*)data->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
}

const gint64* DhNbtInstance::get_long_array(int& len)
{
    auto data = (NbtData*)this->current_nbt->data;
    if(is_type(DH_TYPE_Long_Array))
    {
        len = data->value_a.len;
        return (gint64*)data->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
}

void DhNbtInstance::set_string(const char* str)
{
    auto data = (NbtData*)this->current_nbt->data;
    free(data->value_a.value);
    data->value_a.value = dh_strdup(str);
    data->value_a.len = strlen(str) + 1;
}

bool DhNbtInstance::save_to_file(const char* pos)
{
    auto root = get_original_nbt();
    int bit = 1;
    size_t len = 0;
#ifndef LIBNBT_USE_LIBDEFLATE
    size_t old_len = 0;
#endif
    uint8_t* data = NULL;
    while(1)
    {
        len = 1 << bit;
        data = (uint8_t*)g_new0(uint8_t, len);
        NBT_Error err;
        int ret = nbt_node_pack_opt(root, data, &len, NBT_Compression_GZIP, &err);
        if(ret == 0)
        {
#ifndef LIBNBT_USE_LIBDEFLATE
            if(old_len != len) // compress not finish due to a bug in old libnbt (in submodule)
            {
                old_len = len;
                g_free(data);
                bit++;
                continue;
            }
#endif
            if(pos)
            {
                GFile* file = g_file_new_for_path(pos);
                if(!file)
                {
                    g_free(data);
                    return false;
                }
                if(!g_file_query_exists(file, NULL))
                    g_file_create(file, G_FILE_CREATE_NONE, NULL, NULL);
                GFileIOStream* fios = g_file_open_readwrite(file, NULL, NULL);
                if(fios)
                {
                    GOutputStream* os = g_io_stream_get_output_stream(G_IO_STREAM(fios));
                    auto ret_d = g_output_stream_write(os, data, len, NULL, NULL);
                    bool ret = (ret_d != -1);
                    g_object_unref(fios);
                    g_free(data);
                    g_object_unref(file);
                    return ret;
                }
                else
                {
                    g_object_unref(file);
                    g_free(data);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if(bit < 63)
        {
            g_free(data);
            bit++; // It might be not enough space
        }
        else
        {
            g_free(data);
            return false;
        }
    }
}

DhNbtInstance DhNbtInstance::dup_current_as_original(bool temp_root)
{
    if(is_type(DH_TYPE_Byte))
        return DhNbtInstance(get_byte(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Short))
        return DhNbtInstance(get_short(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Int))
        return DhNbtInstance(get_int(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Long))
        return DhNbtInstance(get_long(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Float))
        return DhNbtInstance(get_float(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Double))
        return DhNbtInstance(get_double(), get_key(), temp_root);
    else if(is_type(DH_TYPE_String))
        return DhNbtInstance(get_string(), get_key(), temp_root);
    else if(is_type(DH_TYPE_Byte_Array))
    {
        int len = 0;
        auto arr = get_byte_array(len);
        return DhNbtInstance(arr, len, get_key(), temp_root);
    }
    else if(is_type(DH_TYPE_Int_Array))
    {
        int len = 0;
        auto arr = get_int_array(len);
        return DhNbtInstance(arr, len, get_key(), temp_root);
    }
    else if(is_type(DH_TYPE_Long_Array))
    {
        int len = 0;
        auto arr = get_long_array(len);
        return DhNbtInstance(arr, len, get_key(), temp_root);
    }
    else
    {
        /* The outer struct */
        DhNbtInstance ret(get_type(), get_key(), temp_root);

        DhNbtInstance origin_child(*this);
        DhNbtInstance prev;

        origin_child.child();
        DhNbtInstance oc(origin_child);
        /* Assign the child value */
        for(; origin_child.is_non_null() ; origin_child.next())
        {
            DhNbtInstance cur = origin_child.dup_current_as_original (true);
            ret.insert_after(prev, cur);
            prev = cur;
        }
        return ret;
    }
}

static void rm_node_internal(DhNbtInstance& child)
{
    g_node_unlink (child.get_current_nbt ());
    nbt_node_free (child.get_current_nbt());
}

bool DhNbtInstance::rm_node(const char* key)
{
    DhNbtInstance child(*this);
    if(child.child(key))
        {
            rm_node_internal(child);
            return true;
        }

    else return false;
}

bool DhNbtInstance::rm_node(int index)
{
    DhNbtInstance child(*this);
    if(child.child(index))
        {
            rm_node_internal(child);
            return true;
        }
    else return false;
}

int DhNbtInstance::child_value()
{
    DhNbtInstance child(*this);
    if(child.child())
    {
        int ret = 0;
        for(; child.is_non_null() ; child.next())
            ret++;
        return ret;
    }
    else return 0;
}

void DhNbtInstance::self_free()
{

}

extern "C"
{
    void* dh_nbt_instance_cpp_new()
    {
        return new DhNbtInstance();
    }

    void  dh_nbt_instance_cpp_free(void* mem)
    {
        DhNbtInstance* instance = dynamic_cast<DhNbtInstance*>((DhNbtInstance*)mem);
        delete instance;
    }
}