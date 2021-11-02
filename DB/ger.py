#!/usr/bin/env python
# coding: utf-8

# In[3]:


import xml.etree.ElementTree as ET
from xml.etree.ElementTree import Element, dump, ElementTree
import pandas as pd


# In[89]:


tree=ET.parse("pubmed21n0001.xml")

root=tree.getroot()

PubmedArticle = root.findall("PubmedArticle")


# In[128]:


#하나의 논문에 여러명의 저자가 존재하는 Author 탭 특성상 Author부분만 따로 정의를 하여 이용

#--------------------------------------Author탭 시작----------------------------------------
LastName=[]; ForeName=[]; Initials=[] #하나의 논문에 여러명의 저자가 있으므로 이중 리스트 사용을 위한 빈 리스트 선언
LN=[]; FN=[]; IN=[]
i=0

for x in PubmedArticle:
    AL=x.find("MedlineCitation").find("Article").find("AuthorList")   
    if(AL!=None):     #저자가 존재하는 경우
        Author=AL.findall('Author')
        LN=[y.findtext("LastName") for y in Author]
        FN=[y.findtext("ForeName") for y in Author]
        IN=[y.findtext("Initials") for y in Author]
        LastName.append(LN); ForeName.append(FN); Initials.append(IN)        
        i=i+1
    else:             #저자가 존재하지 않는 경우
        LastName.append(""); ForeName.append(""); Initials.append(""); 
#--------------------------------------Author탭 끝----------------------------------------
    
PMID=[x.findtext(".//PMID") for x in PubmedArticle]
ArticleTitle=[x.findtext(".//ArticleTitle") for x in PubmedArticle]
Affiliation=[x.findtext(".//Affiliation") for x in PubmedArticle]
Year=[x.findtext(".//Year") for x in PubmedArticle]
Month=[x.findtext(".//Month") for x in PubmedArticle]
Day=[x.findtext(".//Day") for x in PubmedArticle]
Journal=[x.findtext(".//ISSN") for x in PubmedArticle]
Abstract=[x.findtext(".//AbstractText") for x in PubmedArticle]
Country=[x.findtext(".//Country") for x in PubmedArticle]


# In[125]:


df=pd.DataFrame({"PMID":PMID,"LastName":LastName, "ForeName":ForeName,"Initials":Initials, "Affiliation":Affiliation})
df.to_csv("./Author.csv") #저자

df2=pd.DataFrame({"Journal":Journal, "Year":Year,"Month":Month,"Day":Day, "Country":Country})
df2.to_csv("./Journal.csv") #저널

df3=pd.DataFrame({"PMID":PMID, "ArticleTitle":ArticleTitle, "Abstract":Abstract, "Journal":Journal})
df3.to_csv("./Article.csv") #논문

