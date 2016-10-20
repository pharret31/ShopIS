#include "categories.h"

Categories::Categories(char *fileName)
{
    m_fileName = fileName;
    buffer.clear();
    updateBuffer();
}

Categories::~Categories()
{
}


void Categories::saveBuffer()
{
    m_file = fopen(m_fileName,"wb");
    for(iter = buffer.begin(); iter != buffer.end(); iter++)
    {
        categories prod = *iter;
        fwrite(&prod,sizeof(prod),1,m_file);
    }

    buffer.clear();

    fclose(m_file);
}

void Categories::updateBuffer()
{
    buffer.clear();

    m_file = fopen(m_fileName,"rb");
    if(m_file == NULL)
    {
        return;
    }

    categories *prod = new categories;
    while(fread(prod,sizeof(categories),1,m_file) == 1)
    {
        buffer.push_back(*prod);
    }

    fclose(m_file);
}
