//
//  Setting.cpp
//  MenuBar_YouTube
//
//  Created by MitsukiGoto on 2020/01/23.
//  Copyright © 2020 五島充輝. All rights reserved.
//

#include "Setting.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "picojson.h"

Setting::Setting(std::string filepath) {
    m_filepath = filepath;
    if(std::fstream fs(m_filepath, std::ios::in); fs.fail()) {
        makeSettingFile("3", "460", "200");
    } else {
        std::string contents;
        if(std::getline(fs, contents); contents.empty()) {
            makeSettingFile("3", "460", "200");
        }
    }
    loadSetting();
}

void Setting::editSetting(std::string first, std::string second, std::string third) {
    if(first.empty()) {
        first = "3";
    } else if(second.empty()) {
        second = "460";
    } else if (third.empty()) {
        third = "200";
    }
    makeSettingFile(first, second, third);
}

std::vector<std::string> Setting::getSetting() {
    std::vector<std::string> settings;
    auto jsonObj = m_jsonObj;
    std::for_each(jsonObj.begin(), jsonObj.end(), [&settings](auto value){
        settings.push_back(value.second.to_str());
    });
    return settings;
}

void Setting::makeSettingFile(std::string f, std::string s, std::string t) {
    std::ofstream ofs(m_filepath, std::ios::out);
    picojson::object obj;
    obj.insert(std::make_pair(m_f_value, picojson::value(f)));
    obj.insert(std::make_pair(m_s_value, picojson::value(s)));
    obj.insert(std::make_pair(m_t_value, picojson::value(t)));
    ofs << picojson::value(obj) << std::endl;
    ofs.close();
}

void Setting::loadSetting() {
    std::string contents, err;
    std::ifstream ifs(m_filepath, std::ios::in);
    std::getline(ifs, contents);
    picojson::value v;
    picojson::parse(v, contents.c_str(), contents.c_str()+strlen(contents.c_str()), &err);
    if(err.empty()) {
        m_jsonObj = v.get<picojson::object>();
    } else {
        std::cout << err << std::endl;
        std::exit(1);
    }
}
