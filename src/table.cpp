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
    std::map<std::string,std::vector<FuncEntry> >::iterator func_it = map_elements.find(func_name);
    
    //if the function doesn't exist in the table.
    if(func_it == map_elements.end())
    {
        std::pair<std::string,std::vector<FuncEntry> > p;
        p.first = func_name;
        p.second = std::vector<FuncEntry>();
        map_elements.insert(p);
    }

    //adds a overload to the function.
    if(lookup(func_name,params_types_ids,return_type_id) == nullptr)
    {
        std::pair<std::string,std::vector<FuncEntry> > p = (*map_elements.find(func_name)); //always will find the func_name.
        std::vector<FuncEntry> *overloads = &p.second;
        FuncEntry f;
        f.params_types_ids = params_types_ids;
        f.return_type_id = return_type_id;
        overloads->push_back(f);
    }
    else
    {
        //function already defined.
    }
}

FuncEntry *FuncTable::lookup(const std::string &func_name, std::vector<int> params_types_ids, int return_type_id)
{
    std::map<std::string,std::vector<FuncEntry> >::iterator func_it = map_elements.find(func_name);
    if(func_it == map_elements.end())
        return nullptr;

    std::pair<std::string,std::vector<FuncEntry> > p = (*func_it);
    FuncEntry *func_entry = nullptr;
    std::vector<FuncEntry> *overloads = &p.second;

    for(std::vector<FuncEntry>::iterator it = overloads->begin(); it != overloads->end(); it++)
    {
        FuncEntry *f = &(*it);
        bool is_equal = true;
        if(f->return_type_id != return_type_id)
        {    
            is_equal = false;
        }
        else if(f->params_types_ids.size() == params_types_ids.size())
        {
            for(unsigned int i = 0; i < params_types_ids.size(); i++)
            {
                if(f->params_types_ids.at(i) != params_types_ids.at(i))
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