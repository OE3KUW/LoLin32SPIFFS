function toggleCheckbox (element) 
{
    var xhr = new XMLHttpRequest();
    if (element.checked)
    {
        xhr.open("GET", "/update?output="+element.id+"&state=1", true);
        document.getElementById(element.id+"s").innerHTML = "ON";
    }
    else 
    {
        xhr.open("GET", "/update?output="+element.id+"&state=0", true);
        document.getElementById(element.id+"s").innerHTML = "OFF";
    }
    xhr.send();
}

function getStatus()
{  
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/states", true);
    xhr.send();
}

function getStates()
{
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() { 

        if (this.readyState == 4 && this.staus == 200)
        {
            
        }    

                                        };
    xhr.open("GET", "/states", true);
    xhr.send();
}

