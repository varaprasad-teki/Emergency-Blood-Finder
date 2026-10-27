let donors =
JSON.parse(
localStorage.getItem("donors")
) || [];

document
.getElementById("registerForm")
.addEventListener("submit", function(e){

e.preventDefault();

let donor = {

name:
document.getElementById("name").value,

age:
document.getElementById("age").value,

blood:
document.getElementById("bloodGroup").value,

phone:
document.getElementById("phone").value,

location:
document.getElementById("location").value

};

donors.push(donor);

localStorage.setItem(
"donors",
JSON.stringify(donors)
);

alert(
"Donor Registered Successfully"
);

this.reset();

});

function findDonor()
{
let blood =
document.getElementById(
"searchBlood"
).value;

let location =
document.getElementById(
"searchLocation"
).value.toLowerCase();

let data =
JSON.parse(
localStorage.getItem("donors")
) || [];

let result = data.filter(
d=>

d.blood === blood &&

d.location
.toLowerCase()
.includes(location)

);

let html =
"<h3>Available Donors</h3>";

if(result.length===0)
{
html +=
"<p>No Donors Found</p>";
}

result.forEach(d=>{

html += `

<div class="donorCard">

<h4>${d.name}</h4>

<p>Blood : ${d.blood}</p>

<p>Age : ${d.age}</p>

<p>Phone : ${d.phone}</p>

<p>Location : ${d.location}</p>

</div>

`;

});

document
.getElementById("result")
.innerHTML = html;
}
