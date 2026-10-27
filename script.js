let donors = [];

document
.getElementById("registerForm")
.addEventListener("submit", function(e)
{

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

alert("Donor Registered Successfully");

this.reset();

});

function findDonor()
{

let blood =
document.getElementById("searchBlood").value;

let data =
JSON.parse(localStorage.getItem("donors")) || [];

let result =
data.filter(
d => d.blood === blood
);

let html =
"<h3>Available Donors</h3>";

result.forEach(d=>{

html += `
<div class="donor">

<h4>${d.name}</h4>

<p>${d.blood}</p>

<p>${d.location}</p>

<p>${d.phone}</p>

</div>
`;

});

document.getElementById(
"result"
).innerHTML = html;

}
