#!/usr/bin/env python
# coding: utf-8

# In[11]:


import csv
from xml.etree.ElementTree import parse

# csv 파일에 속성 작성하기
f = open('C:\\Users\\주혁\\Desktop\\xml_parsing.csv','a',encoding ="utf-8", newline='' )
wr = csv.writer(f)
wr.writerow(["PMID", "ArticleTitle", "Author(LastName)", "Author(LastName)", "Author(ForeName)", "Affiliation",
             "PubMedPubDate(Year)","PubMedPubDate(Month)", "PubMedPubDate(Day)",
             "ISSN", "Journal Title", "Country", "Abstract"])
f.close()

# 파싱 후 루트노드 가져옴
tree = parse('C:\\Users\\주혁\\Desktop\\pubmed21n0001.xml')
root = tree.getroot()


# 내용 작성(없는 값은 NULL로 대체)
PubmedArticle = root.findall("PubmedArticle")

for i in PubmedArticle:
    row = []
    
    MedlineCitation = i.find("MedlineCitation")
    row.append(MedlineCitation.findtext("PMID"))
    
    if i.find("MedlineCitation/Article") != None:
        Article = i.find("MedlineCitation/Article")
        row.append(Article.findtext("ArticleTitle"))
    
    else:
        row.append("NULL")
    
    if i.find("MedlineCitation/Article/AuthorList/Author") != None:
        Author = i.find("MedlineCitation/Article/AuthorList/Author")
        row += [Author.findtext("LastName"),  Author.findtext("ForeName"), Author.findtext("Initials")]
        
    
    else:
        row += (["NULL", "NULL", "NULL"])
             
    if i.find("MedlineCitation/Article/AuthorList/Author/AffiliationInfo") != None:
        AffiliationInfo = i.find("MedlineCitation/Article/AuthorList/Author/AffiliationInfo")
        row.append(AffiliationInfo.findtext("Affiliation"))
        
    else:
        row.append("NULL")
        
    if i.find("PubmedData/History/PubMedPubDate") != None:
        PubMedPubDate = i.find("PubmedData/History/PubMedPubDate")
        row += [PubMedPubDate.findtext("Year"), PubMedPubDate.findtext("Month"), PubMedPubDate.findtext("Day")]
        
        
    else:
        row += ["NULL", "NULL", "NULL"]
       
    
    if i.find("MedlineCitation/Article/Journal") != None:
        Journal = i.find("MedlineCitation/Article/Journal")
        row.append(Journal.findtext("ISSN"))
        row.append(Journal.findtext("Title"))
        
    else:
        row += ["NULL", "NULL"]
        
        
    if i.find("MedlineCitation/MedlineJournalInfo") != None:
        MedlineJournalInfo = i.find("MedlineCitation/MedlineJournalInfo")
        row.append(MedlineJournalInfo.findtext("Country"))
        
    else:
        row.append("NULL")
        
    if i.find("MedlineCitation/Article/Abstract") != None:
        Abstract = i.find("MedlineCitation/Article/Abstract")
        row.append(Abstract.findtext("AbstractText"))
        
    else:
        row.append("NULL")
        
    f = open('C:\\Users\\주혁\\Desktop\\xml_parsing.csv','a',encoding ="utf-8", newline='' )
    wr = csv.writer(f)
    wr.writerow(row)
    f.close()
 
    
    
    




# In[ ]:




