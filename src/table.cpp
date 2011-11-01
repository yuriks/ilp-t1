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

const TypeEntry* TypeTable::lookup(const std::string &type_name) const
{
    for (auto it = elements.begin(); it != elements.end(); it++) {
        if (it->type_name == type_name) {
            return &*it;
        }
    }
    return nullptr;
}

const TypeEntry* TypeTable::lookup(int type_id) const
{
    if (type_id < (signed int)elements.size())
        return &elements[type_id];
    else
        return nullptr;
}

void TypeTable::print() const
{
    std::cout << "Types Table" << std::endl << std::endl;
    for(auto it = elements.begin(); it != elements.end(); it++)
    {
        std::cout << (*it).type_name << " id: " << (*it).id << std::endl;
    }
    std::cout << std::endl;
}

/* FuncTable */
void FuncTable::insert(const std::string &func_name, const std::vector<int>& params_types_ids, int return_type_id)
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
    if (lookup(func_name, params_types_ids, f) == false) {
        std::map<std::string,std::vector<FuncEntry> >::iterator func_it = map_elements.find(func_name); //always will find the func_name.
        auto& p = *func_it;

        FuncEntry f;
        f.params_types_ids = params_types_ids;
        f.return_type_id = return_type_id;
        p.second.push_back(f);
    } else {
        //function already defined.
    }
}

//return true if found and copies the result to func_entry.
bool FuncTable::lookup(const std::string &func_name, const std::vector<int>& params_types_ids, FuncEntry &func_entry) const
{
    auto func_it = map_elements.find(func_name);
    if (func_it == map_elements.end())
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

void FuncTable::print(const TypeTable& type_table) const
{
    std::cout << "Functions Table" << std::endl << std::endl;
    for (auto func_it = map_elements.begin(); func_it != map_elements.end(); func_it++) {
        auto& pair = *func_it;
        const std::vector<FuncEntry>& overloads = pair.second;
        for (auto ovit = overloads.begin(); ovit != overloads.end(); ovit++) {
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
    std::cout << std::endl;
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

const VarEntry* VarTable::lookup(const std::string &var_name) const
{
    for(auto it = elements.begin(); it != elements.end(); it++) {
        if(it->var_name == var_name) {
            return &*it;
        }
    }

    return nullptr;
}

void VarTable::print(const TypeTable& type_table) const
{
    std::cout << "Variables Table" << std::endl << std::endl;
    /*std::cout << "+-----------------------------+" << std::endl;
    std::cout << "|        Variables Table      |" << std::endl;
    std::cout << "+--------------+--------------+" << std::endl;
    std::cout << "|     Name     |     Type     |" << std::endl;
    std::cout << "+--------------+--------------+" << std::endl;*/
    for(unsigned int i = 0; i < elements.size(); i++)
    {
        std::string vname = elements.at(i).var_name;
        std::string type = toStringFromTypeId(type_table,elements.at(i).type_id);
        
        std::cout << vname << " " << type << std::endl;
    }
    std::cout << std::endl;
}

//return -1 if the type is not defined.
int toTypeId(const TypeTable& type_table, const std::string& type_name)
{
    return type_table.lookup(type_name)->id;
}

//return -1 for not defined types.
std::vector<int> toTypeIds(const TypeTable& type_table, const std::vector<std::string>& type_list)
{
    std::vector<int> res;
    for(auto it = type_list.begin(); it != type_list.end(); it++) {
        res.push_back(toTypeId(type_table, *it));
    }

    return res;
}

std::string toStringFromTypeId(const TypeTable& type_table, int type_id)
{
    return type_table.lookup(type_id)->type_name;
}

}//end of namespace table.