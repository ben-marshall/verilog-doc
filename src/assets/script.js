
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
    listData = listData.sort(function(a,b){
        if(a.id>=b.id){return 1;}
        else {return -1;}
    });

    var i;
    for(i = 0; i < listData.length; i++)
    {
        var item = listData[i];

        var toadd = document.createElement("li");
        var div1 = document.createElement("span");
        var div2 = document.createElement("span");
        
        div1.innerHTML = "<a href='module.html?m="+item.id+"'>"+item.id+"</a>";
        div1.setAttribute("class","item");

        div2.innerHTML = item.file + " - line <b>"+item.line+"</b>";
        div2.style.float='right';

        toadd.appendChild(div1);
        toadd.appendChild(div2);
        container.appendChild(toadd);
    }
}

function veridoc_render_module_hierarchy(
    rootModule,
    container
){
    var tr = document.createElement("div");
    var link = document.createElement("a");
    var newcontainer = document.createElement("span");
    link.innerText = rootModule.id;
    link.setAttribute("href", "module.html?m="+rootModule.id);

    tr.appendChild(link);
    tr.appendChild(newcontainer);

    var i;

    rootModule.children = rootModule.children.sort(function(a,b){
        if(a.id>=b.id){return 1;}
        else {return -1;}
    });

    for(i = 0; i < rootModule.children.length; i++)
    {
        var ta = veridoc_render_module_hierarchy(rootModule.children[i],
                                                                    container);
        newcontainer.appendChild(ta);
    }
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
        container.appendChild( 
            veridoc_render_module_hierarchy(listData[0], container));  
    } else {
        container.innerText = "Error: unknown list type: "+listType;
    }
}

function veridoc_toggle_visible(elem_id){
    var elem  = document.getElementById(elem_id);
    var indicator = document.getElementById(elem_id+"_exp");
    if(elem.style.display != 'none'){
        elem.style.display = 'none';
        indicator.innerText="[+]";
    } else {
        elem.style.display = 'inline';
        indicator.innerText="[-]";
    }
}


function veridoc_new_module_section(
    title,
    data
){
    if(data != null)
    {
        var content = document.createElement("div"); 
        var count   = data.count;

        var cid     = "pcontent_"+title.replace(" ","_");
        content.setAttribute("id",cid);

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
        sectionTitle.innerHTML = "<span id='"+cid+"_exp' class='monospace'>[+]</span> "+ 
                                 title + 
                                 "&nbsp;&nbsp; " +
                                 "("+count+")";
        
        sectionTitle.setAttribute("onclick","veridoc_toggle_visible('"+cid+"')");
        content.style.display = 'none'; // Hide Initially.
        content.appendChild(data.content);
        sectionContent.appendChild(content);
    }
}

/*!
Creates a table display of module ports
*/
function veridoc_render_module_ports(
    data
){
    var table_i = "<thead><td>Name</td><td>Type</td><td>Width</td><td>Direction</td><td>Description</td></thead>";

    for(i = 0; i < data.ports.length; i++)
    {
        var port = data.ports[i];

        table_i += "<tr>";
        table_i += "<td class='monospace'>"+port.name+"</td>";
        table_i += "<td>"+port.type+"</td>";
        table_i += "<td>"+port.range+"</td>";
        table_i += "<td>"+port.direction+"</td>";
        table_i += "<td>None</td>";
        table_i += "</tr>";
    }

    var tr = document.createElement("table");
    tr.innerHTML = table_i;

    return {content:tr,count:data.ports.length};
}


/*!
Creates a table display of module parameters
*/
function veridoc_render_module_parameters(
    data
){
    if(data.parameters.length == 0){return null;}

    var tr = document.createElement("table");
    var header = document.createElement("thead");i
    header.innerHTML = "<td>Name</td><td>Default Value</td><td>Brief</td>";

    for(i = 0; i < data.parameters.length; i++)
    {
        var param = data.parameters[i];
        var row = document.createElement("tr");
        
        row.innerHTML="<td>"+param.name + "</td><td>" + 
                      param.default     + "</td><td>" + 
                      param.brief       + "</td>";

        tr.appendChild(row);
    }

    return {content:tr,count:data.parameters.length};
}

/*!
Creates a table display of module nets 
*/
function veridoc_render_module_nets(
    data
){
    var nets = data.declarations.nets;
    if(nets.length == 0){return null;}

    var tr = document.createElement("table");
    var header = document.createElement("thead");
    header.innerHTML = "<td>Name</td>"+
                       "<td>Type</td>"+
                       "<td>Width</td>"+
                       "<td>Vectored</td>"+
                       "<td>Scalared</td>"+
                       "<td>Signed</td>"+
                       "<td>Description</td>";
    tr.appendChild(header);

    for(i = 0; i < nets.length; i++)
    {
        var net= nets[i];
        var row = document.createElement("tr");
        
        row.innerHTML="<td><span class='codeify'>"+net.name+"</span></td>"+
                      "<td>"+net.nettype+"</td>"+
                      "<td>"+net.range+"</td>"+
                      "<td>"+(net.vectored ? "True" : "False") + "</td>"+
                      "<td>"+(net.scalared ? "True" : "False") + "</td>"+
                      "<td>"+(net.signed   ? "True" : "False") + "</td>"+
                      "<td>"+net.brief+"</td>";
        tr.appendChild(row);
    }

    return {content:tr,count:nets.length};
}


/*!
Creates a table display of module regs 
*/
function veridoc_render_module_regs(
    data
){
    var regs = data.declarations.regs;
    if(regs.length == 0){return null;}

    var tr = document.createElement("table");
    var header = document.createElement("thead");
    header.innerHTML = "<td>Name</td>"+
                       "<td>Type</td>"+
                       "<td>Width</td>"+
                       "<td>Signed</td>"+
                       "<td>Description</td>";
    tr.appendChild(header);

    for(i = 0; i < regs.length; i++)
    {
        var reg= regs[i];
        var row = document.createElement("tr");
        
        row.innerHTML="<td><span class='codeify'>"+reg.name+"</span></td>"+
                      "<td>"+reg.nettype+"</td>"+
                      "<td>"+reg.range+"</td>"+
                      "<td>"+(reg.signed   ? "True" : "False") + "</td>"+
                      "<td>"+reg.brief+"</td>";
        tr.appendChild(row);
    }

    return {content:tr,count:regs.length};
}

/*!
Creates a table display of module variables
*/
function veridoc_render_module_vars(
    data
){
    var vars = data.declarations.vars;
    if(vars.length == 0){return null;}

    var tr = document.createElement("table");
    var header = document.createElement("thead");
    header.innerHTML = "<td>Name</td>"+
                       "<td>Type</td>"+
                       "<td>Description</td>";
    tr.appendChild(header);

    for(i = 0; i < vars.length; i++)
    {
        var v =vars[i];
        var row = document.createElement("tr");
        
        row.innerHTML="<td><span class='codeify'>"+v.name+"</span></td>"+
                      "<td>"+v.type+"</td>"+
                      "<td>"+v.brief+"</td>";
        tr.appendChild(row);
    }

    return {content:tr,count:vars.length};
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
    
    var ports  = veridoc_render_module_ports(data);
    var params = veridoc_render_module_parameters(data);
    var nets   = veridoc_render_module_nets(data);
    var regs   = veridoc_render_module_regs(data);
    var vars   = veridoc_render_module_vars(data);

    veridoc_new_module_section("Parameters", params);
    veridoc_new_module_section("Ports", ports);
    veridoc_new_module_section("Nets", nets);
    veridoc_new_module_section("Regs", regs);
    veridoc_new_module_section("Variables", vars);
}
