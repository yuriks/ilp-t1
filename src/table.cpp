#include "table.hpp"

using namespace table;

/* TypeTable */
void TypeTable::insert(const std::string &type_name)
{
    if(lookup(type_name) == nullptr)
    {
        TypeEntry t;
        t.type_name = type_name;
        t.type_id = (int)elements.size();
        elements.push_back(t);
    }
    else
    {
        //type already in the table.
    }
}

TypeEntry *TypeTable::lookup(const std::string &type_name)
{
    TypeEntry *type_entry = nullptr;

    for(std::vector<TypeEntry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        if((*it).type_name == type_name)
        {
            type_entry = &(*it);
            break;
        }
    }

    return type_entry;
}

TypeEntry *TypeTable::lookup(int type_id)
{
    TypeEntry *type_entry = nullptr;

    for(std::vector<TypeEntry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        if((*it).type_id == type_id)
        {
            type_entry = &(*it);
            break;
        }
    }

    return type_entry;
}

/* FuncTable */
void FuncTable::insert(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id)
{
    if(lookup(func_name,params_types_ids,return_type_id) == nullptr)
    {
        FuncEntry f;
        f.func_name = func_name;
        f.params_types_ids = params_types_ids;
        f.return_type_id = return_type_id;
        elements.push_back(f);
    }
    else
    {
        //function already defined.
    }
}

FuncEntry *FuncTable::lookup(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id)
{
    FuncEntry *func_entry = nullptr;

    for(std::vector<FuncEntry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        FuncEntry *f = &(*it);
        if(f->func_name == func_name)
        {
            bool is_equal = true;
            if(return_type_id != f->return_type_id)
            {
                is_equal = false;
            }
            //verify if the param lists are the same and in the same order.
            else if(params_types_ids.size() == f->params_types_ids.size())
            {
                for(unsigned int i = 0; i < params_types_ids.size(); i++)
                {
                    if(params_types_ids.at(i) != f->params_types_ids.at(i))
                    {
                        is_equal = false;
                        break;
                    }
                }
            }
            else
                is_equal = false;

            if(is_equal)
            {
                func_entry = f;
                break;
            }
        }
    }

    return func_entry;
}

/* VarTable */
void VarTable::insert(const std::string &var_name, int type_id)
{
    if(lookup(var_name) == nullptr)
    {
        VarEntry v;
        v.type_id = type_id;
        v.var_name = var_name;

        elements.push_back(v);
    }
    else
    {
        //variable already defined.
    }
}

VarEntry *VarTable::lookup(const std::string &var_name)
{
    VarEntry *var_entry = nullptr;
    for(std::vector<VarEntry>::iterator it = elements.begin(); it != elements.end(); it++)
    {
        if((*it).var_name == var_name)
        {
            var_entry = &(*it);
            break;
        }
    }

    return var_entry;
}