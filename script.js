const STORAGE_KEY = "lifelink_donors";
const EMERGENCY_KEY = "lifelink_emergencies";

const DEFAULT_DONORS = [
  {
    id: 1,
    name: "Aarav Kumar",
    age: 29,
    blood: "O+",
    phone: "9876543210",
    location: "Hyderabad",
    latitude: 17.3850,
    longitude: 78.4867,
    availability: "available",
    lastDonationDays: 40,
    donationsCount: 6,
    createdAt: Date.now()
  },
  {
    id: 2,
    name: "Meera Nair",
    age: 31,
    blood: "A+",
    phone: "9123456780",
    location: "Banjara Hills",
    latitude: 17.4153,
    longitude: 78.4387,
    availability: "available",
    lastDonationDays: 55,
    donationsCount: 4,
    createdAt: Date.now()
  },
  {
    id: 3,
    name: "Rohit Sharma",
    age: 27,
    blood: "B+",
    phone: "9988776655",
    location: "Secunderabad",
    latitude: 17.4399,
    longitude: 78.4983,
    availability: "busy",
    lastDonationDays: 20,
    donationsCount: 3,
    createdAt: Date.now()
  }
];

const DEFAULT_EMERGENCIES = [
  {
    id: 1,
    patient: "Emergency ICU - Nizamabad",
    bloodGroup: "O+",
    location: "Nizamabad",
    note: "Urgent surgery in progress",
    createdAt: Date.now()
  },
  {
    id: 2,
    patient: "City Trauma Unit",
    bloodGroup: "A+",
    location: "Madhapur",
    note: "Multiple accident victims",
    createdAt: Date.now()
  }
];

let map;
let markers = [];
let mapLoaded = false;

function loadDonors() {
  const stored = localStorage.getItem(STORAGE_KEY);
  if (!stored) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(DEFAULT_DONORS));
    return [...DEFAULT_DONORS];
  }

  try {
    const parsed = JSON.parse(stored);
    return Array.isArray(parsed) && parsed.length ? parsed : [...DEFAULT_DONORS];
  } catch (error) {
    return [...DEFAULT_DONORS];
  }
}

function loadEmergencies() {
  const stored = localStorage.getItem(EMERGENCY_KEY);
  if (!stored) {
    localStorage.setItem(EMERGENCY_KEY, JSON.stringify(DEFAULT_EMERGENCIES));
    return [...DEFAULT_EMERGENCIES];
  }

  try {
    const parsed = JSON.parse(stored);
    return Array.isArray(parsed) && parsed.length ? parsed : [...DEFAULT_EMERGENCIES];
  } catch (error) {
    return [...DEFAULT_EMERGENCIES];
  }
}

function saveDonors(donors) {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(donors));
}

function saveEmergencies(emergencies) {
  localStorage.setItem(EMERGENCY_KEY, JSON.stringify(emergencies));
}

function renderStats() {
  const donors = loadDonors();
  const emergencies = loadEmergencies();

  document.getElementById("totalDonorsStat").textContent = donors.length;
  document.getElementById("dashboardDonors").textContent = donors.length;
  document.getElementById("dashboardRequests").textContent = donors.length + 12;
  document.getElementById("dashboardAlerts").textContent = emergencies.length;
  document.getElementById("alertsStat").textContent = emergencies.length;
  document.getElementById("livesSavedStat").textContent = 18500 + donors.length * 18;
  document.getElementById("dashboardNearby").textContent = donors.filter(d => d.availability === "available").length;
}

function renderEmergencyList() {
  const emergencies = loadEmergencies();
  const list = document.getElementById("emergencyList");

  if (!emergencies.length) {
    list.innerHTML = '<div class="alert-item"><strong>No emergency alerts</strong><p>All systems are stable.</p></div>';
    return;
  }

  list.innerHTML = emergencies
    .slice(0, 4)
    .map(
      (e) => `
        <div class="alert-item">
          <strong>${e.patient}</strong>
          <p><strong>Blood:</strong> ${e.bloodGroup} · <strong>Location:</strong> ${e.location}</p>
          <p>${e.note}</p>
        </div>
      `
    )
    .join("");
}

function setStatusBadge(status) {
  return `<span class="badge ${status}">${status}</span>`;
}

function renderSearchResults(results) {
  const resultContainer = document.getElementById("result");

  if (!results.length) {
    resultContainer.innerHTML = `
      <div class="result-card">
        <strong>No donors found</strong>
        <div class="result-meta">
          <span>Try another blood group or location.</span>
        </div>
      </div>
    `;
    return;
  }

  resultContainer.innerHTML = results
    .map(
      (donor) => `
        <div class="result-card">
          <strong>${donor.name}</strong>
          <div class="result-meta">
            <span>${donor.blood} · ${donor.age} yrs</span>
            <span>${donor.location}</span>
          </div>
          <div class="result-meta">
            <span>${donor.phone}</span>
            <span>${donor.distanceKm ? donor.distanceKm.toFixed(1) + " km away" : "Location available"}</span>
          </div>
          <div class="result-meta">
            <span>Donations: ${donor.donationsCount}</span>
            ${setStatusBadge(donor.availability)}
          </div>
        </div>
      `
    )
    .join("");
}

function validateForm(fields) {
  for (const field of fields) {
    if (!field || field.trim() === "") {
      return false;
    }
  }
  return true;
}

function getDistanceKm(lat1, lon1, lat2, lon2) {
  const R = 6371;
  const dLat = (lat2 - lat1) * (Math.PI / 180);
  const dLon = (lon2 - lon1) * (Math.PI / 180);
  const a =
    Math.sin(dLat / 2) * Math.sin(dLat / 2) +
    Math.cos(lat1 * (Math.PI / 180)) * Math.cos(lat2 * (Math.PI / 180)) *
    Math.sin(dLon / 2) * Math.sin(dLon / 2);
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c;
}

function isBloodCompatible(requestBlood, donorBlood) {
  const compatibilityMap = {
    "O-": ["O-"],
    "O+": ["O-", "O+"],
    "A-": ["A-", "O-"],
    "A+": ["A-", "O-", "A+", "O+"],
    "B-": ["B-", "O-"],
    "B+": ["B-", "O-", "B+", "O+"],
    "AB-": ["AB-", "A-", "B-", "O-"],
    "AB+": ["AB-", "A-", "B-", "O-", "AB+", "A+", "B+", "O+"]
  };

  return compatibilityMap[donorBlood]?.includes(requestBlood) || false;
}

function donorRecommendationScore(donor) {
  let score = 60;
  if (donor.availability === "available") score += 18;
  if (donor.lastDonationDays >= 90) score += 12;
  if (donor.donationsCount >= 3) score += 10;
  return Math.min(score, 99);
}

function findDonorsByQuery(filters = {}) {
  const donors = loadDonors();
  let results = donors.filter((donor) => {
    const bloodMatch = !filters.blood || donor.blood === filters.blood;
    const locationMatch = !filters.location || donor.location.toLowerCase().includes(filters.location.toLowerCase());
    const availabilityMatch = !filters.availableOnly || donor.availability === "available";
    return bloodMatch && locationMatch && availabilityMatch;
  });

  if (filters.nearMe && filters.userCoords) {
    results = results
      .map((donor) => {
        const distance = donor.latitude && donor.longitude
          ? getDistanceKm(filters.userCoords.latitude, filters.userCoords.longitude, donor.latitude, donor.longitude)
          : Number.MAX_SAFE_INTEGER;
        return { ...donor, distanceKm: distance };
      })
      .filter((donor) => donor.distanceKm <= 30)
      .sort((a, b) => a.distanceKm - b.distanceKm);
  } else if (filters.blood || filters.location) {
    results = results.map((donor) => ({ ...donor, distanceKm: null }));
  }

  return results;
}

function initMap() {
  const mapContainer = document.getElementById("map");
  if (!mapContainer) return;

  const key = window.GOOGLE_MAPS_API_KEY;
  if (!key || key.includes("YOUR") || key === "demo") {
    mapContainer.innerHTML = "<div style='display:grid;place-items:center;height:100%;color:#a6b1c6;'>Google Maps is unavailable because no valid API key is configured.</div>";
    return;
  }

  if (typeof google === "undefined" || !google.maps) {
    mapContainer.innerHTML = "<div style='display:grid;place-items:center;height:100%;color:#a6b1c6;'>Loading Google Maps…</div>";
    return;
  }

  map = new google.maps.Map(mapContainer, {
    center: { lat: 17.3850, lng: 78.4867 },
    zoom: 11,
    styles: [{
      featureType: "all",
      elementType: "geometry",
      stylers: [{ color: "#1f2d3d" }]
    }, {
      featureType: "all",
      elementType: "labels.text.fill",
      stylers: [{ color: "#edf2ff" }]
    }, {
      featureType: "all",
      elementType: "labels.text.stroke",
      stylers: [{ color: "#0b1220" }]
    }]
  });

  mapLoaded = true;
  showMarkers(loadDonors().slice(0, 6));
}

function clearMarkers() {
  markers.forEach((marker) => marker.setMap(null));
  markers = [];
}

function showMarkers(donors) {
  if (!map || !mapLoaded || !donors.length) return;

  clearMarkers();

  donors.forEach((donor) => {
    if (donor.latitude && donor.longitude) {
      const marker = new google.maps.Marker({
        map,
        position: { lat: donor.latitude, lng: donor.longitude },
        title: `${donor.name} - ${donor.blood}`,
        icon: {
          url: "https://maps.google.com/mapfiles/ms/icons/red-dot.png"
        }
      });

      const infoWindow = new google.maps.InfoWindow({
        content: `<div style="color:#111;padding:8px 10px;"><strong>${donor.name}</strong><br>${donor.blood} · ${donor.location}<br>${donor.availability}</div>`
      });

      marker.addListener("click", () => infoWindow.open({ anchor: marker, map }));
      markers.push(marker);
    }
  });

  if (donors[0]?.latitude && donors[0]?.longitude) {
    map.setCenter({ lat: donors[0].latitude, lng: donors[0].longitude });
  }
}

function getUserLocation(callback) {
  if (!navigator.geolocation) {
    alert("Geolocation is not supported by this browser.");
    return;
  }

  navigator.geolocation.getCurrentPosition(
    (position) => {
      const coords = {
        latitude: position.coords.latitude,
        longitude: position.coords.longitude
      };
      document.getElementById("latitude").value = coords.latitude;
      document.getElementById("longitude").value = coords.longitude;
      callback(coords);
    },
    () => {
      alert("Location access was denied. Please enter coordinates manually.");
    },
    {
      enableHighAccuracy: true,
      timeout: 10000,
      maximumAge: 0
    }
  );
}

function handleRegistration(event) {
  event.preventDefault();

  const donor = {
    id: Date.now(),
    name: document.getElementById("name").value.trim(),
    age: Number(document.getElementById("age").value),
    blood: document.getElementById("bloodGroup").value,
    phone: document.getElementById("phone").value.trim(),
    location: document.getElementById("location").value.trim(),
    latitude: parseFloat(document.getElementById("latitude").value) || null,
    longitude: parseFloat(document.getElementById("longitude").value) || null,
    availability: document.getElementById("availability").value,
    lastDonationDays: Number(document.getElementById("lastDonationDays").value),
    donationsCount: Number(document.getElementById("donationsCount").value),
    createdAt: Date.now()
  };

  if (!validateForm([donor.name, donor.blood, donor.phone, donor.location])) {
    alert("Please fill in all required donor details.");
    return;
  }

  if (donor.age < 18 || donor.age > 65) {
    alert("Donor age must be between 18 and 65 years.");
    return;
  }

  const donors = loadDonors();
  donors.push(donor);
  saveDonors(donors);

  renderStats();
  renderSearchResults(findDonorsByQuery({ blood: donor.blood, location: donor.location }));
  showMarkers(donors.slice(-6));
  event.target.reset();
  alert("Donor registered successfully.");
}

function handleSearch() {
  const blood = document.getElementById("searchBlood").value;
  const location = document.getElementById("searchLocation").value.trim();
  const results = findDonorsByQuery({ blood, location });
  renderSearchResults(results);
  showMarkers(results.slice(0, 6));
}

function handleNearMe() {
  const blood = document.getElementById("searchBlood").value;
  getUserLocation((coords) => {
    const results = findDonorsByQuery({
      blood,
      nearMe: true,
      userCoords: coords,
      availableOnly: true
    });

    renderSearchResults(results);
    showMarkers(results.slice(0, 10));
  });
}

function handleEmergencyRequest(event) {
  event.preventDefault();

  const emergency = {
    id: Date.now(),
    patient: document.getElementById("requesterName").value.trim(),
    bloodGroup: document.getElementById("requestBloodGroup").value,
    location: document.getElementById("requestLocation").value.trim(),
    note: document.getElementById("requestNotes").value.trim() || "Immediate support required",
    createdAt: Date.now()
  };

  if (!validateForm([emergency.patient, emergency.bloodGroup, emergency.location])) {
    alert("Please complete all emergency request details.");
    return;
  }

  const emergencies = loadEmergencies();
  emergencies.unshift(emergency);
  saveEmergencies(emergencies);
  renderEmergencyList();
  renderStats();

  event.target.reset();
  alert("Emergency blood alert broadcast successfully.");
}

function registerEventHandlers() {
  document.getElementById("registerForm").addEventListener("submit", handleRegistration);
  document.getElementById("searchBtn").addEventListener("click", handleSearch);
  document.getElementById("nearMeBtn").addEventListener("click", handleNearMe);
  document.getElementById("locateBtn").addEventListener("click", () => {
    getUserLocation(() => {});
  });
  document.getElementById("emergencyForm").addEventListener("submit", handleEmergencyRequest);
}

function bootstrap() {
  renderStats();
  renderEmergencyList();
  renderSearchResults(loadDonors().slice(0, 3));
  registerEventHandlers();
  initMap();
}

window.addEventListener("DOMContentLoaded", bootstrap);
window.addEventListener("load", initMap);
