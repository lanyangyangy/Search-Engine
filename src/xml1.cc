// #include <iostream>
// #include <stdio.h>
// #include "tinyxml/tinyxml.h"
// // using namespace::tinyxml2;
// void create_XML() {
// 	printf("\n----- create_XML -----\n"); 

// 	//新建一个xml文件
// 	// 定义一个TiXmlDocument类指针
// 	TiXmlDocument* tinyXmlDoc = new TiXmlDocument();



// 	// xml的声明(三个属性：版本，编码格式，保留空串即可)
// 	TiXmlDeclaration* tinyXmlDeclare = new TiXmlDeclaration("1.0", "utf-8", "");	// 声明头部格式
// 	// 插入文档类中
// 	tinyXmlDoc->LinkEndChild(tinyXmlDeclare);



// 	// 根节点
// 	TiXmlElement* Library = new TiXmlElement("Library");
// 	tinyXmlDoc->LinkEndChild(Library);		// 把根节点插入到文档类中



// 	// 创建Book节点
// 	TiXmlElement *Book = new TiXmlElement("Book");
// 	TiXmlText *bookText = new TiXmlText("书本");	// 创建文本
// 	Book->LinkEndChild(bookText);	// 给Book节点添加文本
// 	Library->LinkEndChild(Book);	// 插入到根节点下


// 	// 创建Book1节点
// 	TiXmlElement *Book1 = new TiXmlElement("Book1");
// 	// 插入属性
// 	Book1->SetAttribute("ID", 1);
// 	Book1->SetAttribute("Name", "水浒传");
// 	Book1->SetAttribute("Price", "64.6");

// 	// 创建Book1的子节点Description
// 	TiXmlElement *Description = new TiXmlElement("Description");
// 	TiXmlText *descriptionText = new TiXmlText("108个拆迁户");		// 创建文本
// 	Description->LinkEndChild(descriptionText);		// 给Description节点添加文本
// 	Book1->LinkEndChild(Description);				// 插入到Book1节点下

// 	// 创建Book1的子节点Page
// 	TiXmlElement *Page = new TiXmlElement("Page");
// 	TiXmlText *pageText = new TiXmlText("100页");		// 创建文本
// 	Page->LinkEndChild(pageText);	// 给Page节点添加文本
// 	Book1->LinkEndChild(Page);		// 插入到Book1节点下

// 	Library->LinkEndChild(Book1);	// 插入到根节点下



// 	// 保存到文件	
// 	bool result = tinyXmlDoc->SaveFile("1.xml");
// 	if (result == true) printf("文件写入成功！\n");
// 	else printf("文件写入失败！\n");

// 	// 打印出来看看
// 	//tinyXmlDoc->Print();

// 	TiXmlPrinter printer;
// 	tinyXmlDoc->Accept(&printer);
// 	printf("%s\n", printer.CStr());
// }

// int main()
// {
//     create_XML();
//     return 0;
// }