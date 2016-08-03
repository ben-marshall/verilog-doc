
/*
Contains all of the functions for the documentation pages.
*/

/*
Gets page URL parameters by name.
*/
function gup( name, url ) {
if (!url) url = location.href;
name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
var regexS = "[\\?&]"+name+"=([^&#]*)";
var regex = new RegExp( regexS );
var results = regex.exec( url );
return results == null ? null : results[1];
}


function veridoc_render_file_list(
    listData,
    container
){
    var toset = ""

    listData = listData.sort(function(a,b){
        if(a.path>=b.path){return 1;}
        else {return -1;}    });

    var i;
    for(i = 0; i < listData.length; i++)
    {
        var item = listData[i];
        
        toset += "<li>";
        if(item.parsed == "1"){
            toset += "<div class='status good'>Found</div>";
            if(item.success== "1"){
                toset += "<div class='status good'>Parsed</div>";
            } else {
                toset += "<div class='status bad'>Syntax Errors</div>";
            }
        } else {
            toset += "<div class='status bad'>Not Found</div>";
        }
        
        toset += "<div class='item'>"+item.path+"</div>";
        toset += "</li>"

    }

    container.innerHTML = toset;
}


function veridoc_render_module_list(
    listData,
    container
){
    var toset = ""

    listData = listData.sort(function(a,b){
        if(a.id>=b.id){return 1;}
        else {return -1;}
    });

    var i;
    for(i = 0; i < listData.length; i++)
    {
        var item = listData[i];
        
        toset += "<li>";
        toset += "<div class='item'><a href='module.html?m="+item.id+"'>"+
                 item.id+"</a></div>";
        toset += "</li>"

    }

    container.innerHTML = toset;
}

function veridoc_render_module_hierarchy(
    rootModule,
    container
){
    var tr = "<div>"+
        "<a href='module.html?m="+rootModule.id+"'>"+rootModule.id+"</a>";

    var i;

    rootModule.children = rootModule.children.sort(function(a,b){
        if(a.id>=b.id){return 1;}
        else {return -1;}
    });

    for(i = 0; i < rootModule.children.length; i++)
    {
        tr +=veridoc_render_module_hierarchy(rootModule.children[i],container);
    }

    tr += "</div>";
    return tr;
}

/*
Responsible for taking a JSON data structure and turning it into a list
on the list.html page.
*/
function veridoc_render_list(
    data
){
    var listType = data.listType;
    var listData = data.listData;
    var listTitle = data.listTitle;
    var listNotes = data.listNotes;

    document.getElementById('list-title').innerText = listTitle;
    document.getElementById('list-notes').innerText = listNotes;

    // This is a <ul> element
    var container = document.getElementById('list-container');
    container.innerHTML = "";

    if(listType == "file-manifest"){
        veridoc_render_file_list(listData, container);
    } else if (listType == "module-manifest") {
        veridoc_render_module_list(listData, container);  
    } else if (listType == "module-hierarchy") {
        container.innerHTML +=
            veridoc_render_module_hierarchy(listData[0], container);  
    } else {
        container.innerText = "Error: unknown list type: "+listType;
    }
}

function veridoc_new_module_section(
    title,
    content
){
    var pageCont = document.getElementById("page-content");
    var section = document.createElement("div");
    var sectionTitle = document.createElement("div");
    var sectionContent = document.createElement("div");
    section.appendChild(sectionTitle);
    section.appendChild(sectionContent);
    pageCont.appendChild(section);
    section.setAttribute("class", "page-section");
    sectionTitle.setAttribute("class", "page-section-title");
    sectionContent.setAttribute("class", "page-section-content");
    sectionTitle.innerHTML = "<span class='monospace'>[-]</span> "+title;
    sectionContent.innerHTML = content;
}


/*!
Responsible for rendering a module page from it's JSON attributes.
*/
function veridoc_render_module(){
    var module = gup("m", location.href);
    
    var data     = veridocModuleInformation;
    var hierCurr = document.getElementById("hier-current");
    var hierKids = document.getElementById("child-list");
    var docTitle = document.getElementById("module-title");
    var docLine  = document.getElementById("module-line");
    var docFile  = document.getElementById("source-file");
    var docBrief = document.getElementById("module-description");

    docTitle.innerText = data.moduleName;
    hierCurr.innerText = data.moduleName;
    docLine.innerText  = data.moduleLine;
    docFile.innerText  = data.moduleFile;
    docBrief.innerText = data.moduleBrief;

    var i;
    for(i = 0; i < data.children.length; i++)
    {
        var li = document.createElement("li");
        var link = document.createElement("a");
        link.setAttribute("href", "module.html?m="+data.children[i].moduleName);
        link.innerText = data.children[i].moduleName;
        li.appendChild(link);
        hierKids.appendChild(li);
    }


    var table = "<table><thead><td>Type</td><td>Width</td><td>Direction</td><td>Name</td><td>Description</td></thead>";

    for(i = 0; i < data.ports.length; i++)
    {
        var port = data.ports[i];

        table += "<tr>";
        table += "<td>"+port.type+"</td>";
        table += "<td>"+port.range+"</td>";
        table += "<td>"+port.direction+"</td>";
        table += "<td class='monospace'>"+port.name+"</td>";
        table += "<td>None</td>";
        table += "</tr>";
    }

    table += "</table>";
    veridoc_new_module_section("Ports", table);
}
