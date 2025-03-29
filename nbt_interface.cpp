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
#include <stdexcept>

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

static void parse_nbt(DhNbtInstance& instance, NBT* nbt, bool tr)
{
    if(tr) parse_nbt_tmp(instance, nbt);
    else   parse_nbt_real(instance, nbt);
}

static bool has_child(DhNbtInstance parent, DhNbtInstance child)
{
    NBT* parent_node = parent.get_current_nbt();
    if(parent_node->child && child.is_non_null())
    {
        NBT* child_node = parent_node->child;
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

DhNbtInstance::DhNbtInstance(NBT* nbt, bool tr)
{
    parse_nbt(*this, nbt, tr);
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
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint16 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Short;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint32 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Int;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint64 val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Long;
    new_nbt->value_i = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(float val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Float;
    new_nbt->value_d = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(double val, const char *key, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Double;
    new_nbt->value_d = val;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const char *val, const char *key, bool temporary_root)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_String;
    new_nbt->value_a.value = dh_strdup(val);
    new_nbt->value_a.len = strlen(val) + 1;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint8 *val, int len, const char *key, bool temporary_root)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int8_t);
    int8_t* new_array = (gint8*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint32 *val, int len, const char *key, bool temporary_root)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int32_t);
    int32_t* new_array = (gint32*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(gint64 *val, int len, const char *key, bool temporary_root)
{
    NBT* new_nbt = ret_non_filled_nbt();
    new_nbt->type = TAG_Byte_Array;
    int byte = len * sizeof(int64_t);
    int64_t* new_array = (gint64*)malloc(byte);
    memcpy(new_array, val, byte);
    new_nbt->value_a.value = new_array;
    new_nbt->value_a.len = len;
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
}

DhNbtInstance::DhNbtInstance(const char *key, DhNbtType type, bool temporary_root)
{
    auto new_nbt = ret_non_filled_nbt();
    new_nbt->type = (NBT_Tags)(type - 1);
    new_nbt->key = key ? dh_strdup(key) : NULL;
    parse_nbt(*this, new_nbt, temporary_root);
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
    if(current_nbt->type == (type - 1)) return true;
    else return false;
}

bool DhNbtInstance::parent()
{
    int len = tree_struct.size();
    current_nbt = tree_struct[len - 1];
    tree_struct.resize(len - 1);
    return true;
}

bool DhNbtInstance::child()
{
    if(is_non_null() && (is_type(DH_TYPE_Compound) || is_type(DH_TYPE_List)))
    {
        tree_struct.push_back(current_nbt);
        current_nbt = current_nbt->child;
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

void DhNbtInstance::goto_root()
{
    current_nbt = get_original_nbt();
    tree_struct.clear();
}

const char* DhNbtInstance::get_key()
{
    if(is_non_null())
        return current_nbt->key;
    else return nullptr;
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
            current_nbt->child = child.get_current_nbt();
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
        if(sibling.is_non_null())
        {
            NBT* sibling_node = sibling.get_current_nbt();
            if(sibling_node->next)
                sibling_node->next->prev = node.get_current_nbt();
            node.get_current_nbt()->next = sibling_node->next;
            node.get_current_nbt()->prev = sibling_node;
            sibling_node->next = node.get_current_nbt();
        }
        else
        {
            if(current_nbt->child)
            {
                node.get_current_nbt()->next = current_nbt->child;
                current_nbt->child->prev = node.get_current_nbt();
            }
            current_nbt->child = node.get_current_nbt();
        }
        return true;
    }
    else return false;
}

bool DhNbtInstance::insert_before(DhNbtInstance sibling, DhNbtInstance node)
{
    if(has_child(*this, sibling))
    {
        if(sibling.is_non_null())
        {
            NBT* sibling_node = sibling.current_nbt;
            if(sibling_node->prev)
            {
                node.current_nbt->prev = sibling_node->prev;
                node.current_nbt->prev->next = node.current_nbt;
                node.current_nbt->next = sibling_node;
                sibling_node->prev = node.current_nbt;
            }
            else
            {
                current_nbt->child = node.current_nbt;
                node.current_nbt->next = sibling_node;
                sibling_node->prev = node.current_nbt;
            }
        }
        else
        {
            if(current_nbt->child)
            {
                NBT* sibling_node = current_nbt->child;
                while(sibling_node->next)
                    sibling_node = sibling_node->next;
                node.current_nbt->prev = sibling_node;
                sibling_node->next = node.current_nbt;
            }
            else current_nbt->child = node.current_nbt;
        }
        return true;
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

gint8 DhNbtInstance::get_byte()
{
    if(is_type(DH_TYPE_Byte)) return current_nbt->value_i;
    else throw std::domain_error("Not the right type!");
}

gint16 DhNbtInstance::get_short()
{
    if(is_type(DH_TYPE_Short)) return current_nbt->value_i;
    else throw std::domain_error("Not the right type!");
}

gint32 DhNbtInstance::get_int()
{
    if(is_type(DH_TYPE_Int)) return current_nbt->value_i;
    else throw std::domain_error("Not the right type!");
}

gint64 DhNbtInstance::get_long()
{
    if(is_type(DH_TYPE_Long)) return current_nbt->value_i;
    else throw std::domain_error("Not the right type!");
}

gint64 DhNbtInstance::get_integer()
{
    auto type = get_type();
    if(type >= DH_TYPE_Byte && type <= DH_TYPE_Long)
        return current_nbt->value_i;
    else throw std::domain_error("Not the right type!");
}

float DhNbtInstance::get_float()
{
    if(is_type(DH_TYPE_Float)) return current_nbt->value_d;
    else throw std::domain_error("Not the right type!");
}

double DhNbtInstance::get_double()
{
    if(is_type(DH_TYPE_Double)) return current_nbt->value_d;
    else throw std::domain_error("Not the right type!");
}

const gchar* DhNbtInstance::get_string()
{
    if(is_type(DH_TYPE_String)) return (const char*)current_nbt->value_a.value;
    else throw std::domain_error("Not the right type!");
}

const gint8* DhNbtInstance::get_byte_array(int& len)
{
    if(is_type(DH_TYPE_Byte_Array))
    {
        len = current_nbt->value_a.len;
        return (gint8*)current_nbt->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
}


const gint32* DhNbtInstance::get_int_array(int& len)
{
    if(is_type(DH_TYPE_Int_Array))
    {
        len = current_nbt->value_a.len;
        return (gint32*)current_nbt->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
}

const gint64* DhNbtInstance::get_long_array(int& len)
{
    if(is_type(DH_TYPE_Long_Array))
    {
        len = current_nbt->value_a.len;
        return (gint64*)current_nbt->value_a.value;
    }
    else throw std::domain_error("Not the right type!");
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