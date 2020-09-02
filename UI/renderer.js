function convertToCSV(objArray) {
  var array = typeof objArray != 'object' ? JSON.parse(objArray) : objArray;
  var str = '';

  for (var i = 0; i < array.length; i++) {
    var line = '';
    for (var index in array[i]) {
      if (line != '') line += ','

      line += array[i][index];
    }

    str += line + '\r\n';
  }

  return str;
}

function exportCSVFile(headers, items, fileTitle) {
  if (headers) {
    items.unshift(headers);
  }

  // Convert Object to JSON
  var jsonObject = JSON.stringify(items);

  var csv = this.convertToCSV(jsonObject);

  var exportedFilenmae = fileTitle + '.csv' || 'export.csv';

  var blob = new Blob([csv], { type: 'text/csv;charset=utf-8;' });
  if (navigator.msSaveBlob) { // IE 10+
    navigator.msSaveBlob(blob, exportedFilenmae);
  } else {
    var link = document.createElement("a");
    if (link.download !== undefined) { // feature detection
      // Browsers that support HTML5 download attribute
      var url = URL.createObjectURL(blob);
      link.setAttribute("href", url);
      link.setAttribute("download", exportedFilenmae);
      link.style.visibility = 'hidden';
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    }
  }
}

function download(itemsNotFormatted, numberquestion) {
  var headers = {
    name: 'Họ Và Tên'.replace(/,/g, ''), // remove commas to avoid errors
    identification: "Mã báo danh",
    rightanswer: "Số câu đúng",
    point: "Điểm"
  };

  var itemsFormatted = [];

  // format the data
  itemsNotFormatted.forEach((item) => {
    itemsFormatted.push({
      name: item.name.replace(/,/g, ''), // remove commas to avoid errors,
      identification: item.identification,
      rightanswer: item.numberrightanswer,
      point: item.numberrightanswer / numberquestion * 10,
    });
  });

  var fileTitle = 'orders'; // or 'my-unique-title'

  exportCSVFile(headers, itemsFormatted, fileTitle); // call the exportCSVFile() function to process the JSON and trigger the download
}

var dataCSV = "";

function show(data) {
  let tabs = "";
  var count = 1;

  for (let r of data.listofpoints) {
    tabs +=
      `<tr>
      <th scope="row">${count++}</th>
      <td>${r.name}</td>
      <td>${r.identification}</td>
      <td>${r.numberrightanswer}/${data.numberofquestions}</td>
      <td>${r.numberrightanswer / data.numberofquestions * 10}</td>
    </tr>`;

    document.getElementById("loading-wrapper-id").style.display = "none";
    document.getElementById("test1").innerHTML = tabs;
    document.getElementById("table-wrapper-id").style.display = "flex";
  }
}

function setFailed(messageFailed) {
  document.getElementById("loading-wrapper-id").style.display = "none";
  document.getElementById("test3").style.display = "flex";
  document.getElementById("failed-box").innerHTML = messageFailed;
}

function isSuccess(response) {
  return response.errorCode === 0;
}

async function getAPI(url, pathFile) {
  pathFile = '/home/baopdh/Downloads/school/Multiple-choice-Marking/images'
  var myHeaders = new Headers();
  myHeaders.append("Content-Type", "text/plain");
  var raw = pathFile;

  var requestOptions = {
    method: 'POST',
    headers: myHeaders,
    body: raw,
    redirect: 'follow'
  };

  try {
    const result = await fetch("http://localhost:8080/mark", requestOptions);
    const jsonData = await result.text();
    const dataObject = JSON.parse(jsonData);

    if (!isSuccess(dataObject)) {
      return setFailed(dataObject.message);
    }

    const getResult = async () => {
      var requestOptions = {
        method: 'GET',
        redirect: 'follow'
      };
      const data = await fetch(url, requestOptions);
      const jsonData = await data.text();
      const dataObject = JSON.parse(jsonData);
  
      if (!isSuccess(dataObject)) {
        return setFailed(dataObject.message);
      }
  
      if (dataObject.data.status === -1) {
        return setFailed(dataObject.data.message);
      } else if (dataObject.data.status === 1 || dataObject.data.status === 2) {
        return setTimeout(() => getResult(), 2000);
      }
  
      dataCSV = dataObject.data.data.listofpoints;
      show(dataObject.data.data);
    }

    getResult();
  } catch (err) {
    setFailed(err);
  }
}

document.getElementById("test2").addEventListener("click", function () {
  document.getElementById("loading-wrapper-id").style.display = "flex";
  document.getElementById("table-wrapper-id").style.display = "none";

  const pathFile = document.getElementById("folder-name-container").textContent;

  getAPI("http://localhost:8080/mark", pathFile);
});

document.getElementById("myFile").addEventListener("change", function () {
  const str = document.getElementById('myFile').files[0].path;
  const pathFile = str.slice(0, str.slice(0, str.lastIndexOf("/")).lastIndexOf("/"));
  document.getElementById("folder-name-container").innerHTML = pathFile;
});

document.getElementById("export-csv").addEventListener("click", function () {
  const itemsNotFormatted = dataCSV;
  download(itemsNotFormatted, 10);
});
