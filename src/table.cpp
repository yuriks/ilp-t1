#include "table.hpp"

#include <iostream>

namespace table
{
/* TypeTable */
void TypeTable::insert(const std::string &type_name)
{
    if(lookup(type_name) == nullptr)
    {
        TypeEntry t;
        t.type_name = type_name;
        t.id = (int)elements.size();
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
        if((*it).id == type_id)
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
    FuncEntry f;
     //adds a overload to the function.
    if(lookup(func_name,params_types_ids,f) == false)
    {
        std::map<std::string,std::vector<FuncEntry> >::iterator func_it = map_elements.find(func_name); //always will find the func_name.
        auto& p = *func_it;

        FuncEntry f;
        f.params_types_ids = params_types_ids;
        f.return_type_id = return_type_id;
        p.second.push_back(f);
    }
    else
    {
        //function already defined.
    }
}

//return true if found and copies the result to func_entry.
bool FuncTable::lookup(const std::string &func_name, std::vector<int> &params_types_ids, FuncEntry &func_entry)
{
    std::map<std::string,std::vector<FuncEntry> >::iterator func_it = map_elements.find(func_name);
    if(func_it == map_elements.end())
        return nullptr;

    std::pair<std::string,std::vector<FuncEntry> > p = *func_it;
    std::vector<FuncEntry> *overloads = &p.second;

    for(std::vector<FuncEntry>::iterator it = overloads->begin(); it != overloads->end(); it++)
    {
        FuncEntry *f = &(*it);
        bool is_equal = true;
        if(f->params_types_ids.size() == params_types_ids.size())
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
            func_entry = *f;
            return true;
        }
    }

    return false;
}

void FuncTable::print(TypeTable *type_table)
{
    std::cout << "Functions Table" << std::endl;
    std::map<std::string,std::vector<FuncEntry> >::iterator func_it;
    for(func_it = map_elements.begin(); func_it != map_elements.end(); func_it++)
    {
        auto& pair = *func_it;
        std::vector<FuncEntry> *overloads = &pair.second;
        for(std::vector<FuncEntry>::iterator ovit = overloads->begin(); ovit != overloads->end(); ovit++)
        {
            std::cout << pair.first << "(";
            std::vector<int> entry_params = (*ovit).params_types_ids;
            for(unsigned int i = 0; i < entry_params.size(); i++)
            {
                std::string param_type = toStringFromTypeId(type_table, entry_params.at(i));
                std::cout << param_type;
                if(i == entry_params.size() - 1)
                    std::cout << ") -> " << toStringFromTypeId(type_table,(*ovit).return_type_id) << std::endl;
                else
                    std::cout << ",";
            }
        }
    }
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

void VarTable::print(TypeTable *type_table)
{
    std::cout << "+-----------------------------+" << std::endl;
    std::cout << "|        Variables Table      |" << std::endl;
    std::cout << "+--------------+--------------+" << std::endl;
    std::cout << "|     Name     |     Type     |" << std::endl;
    std::cout << "+--------------+--------------+" << std::endl;
    for(unsigned int i = 0; i < elements.size(); i++)
    {
        std::string vname = elements.at(i).var_name;
        std::string type = toStringFromTypeId(type_table,elements.at(i).type_id);
        
        std::cout << vname << " " << type << std::endl;
    }
}

//return -1 if the type is not defined.
int toTypeId(TypeTable *type_table, std::string type_name)
{
    return type_table->lookup(type_name)->id;
}
//return -1 for not defined types.
std::vector<int> toTypeIds(TypeTable *type_table, std::vector<std::string> type_list)
{
    std::vector<int> res;
    for(std::vector<std::string>::iterator it = type_list.begin(); it != type_list.end(); it++)
    {
        res.push_back(toTypeId(type_table,*it));
    }

    return res;
}

std::string toStringFromTypeId(TypeTable *type_table, int type_id)
{
    return type_table->lookup(type_id)->type_name;
}

}//end of namespace table.