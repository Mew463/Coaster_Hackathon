document.addEventListener("DOMContentLoaded", () => {
    const goodLock = document.getElementById("goodLock");
    const badLock = document.getElementById("badLock");
    
    const menuButtons = [
        'lockButton',
        'musicButton',
        'cupLocationButton'
    ];
    const functionMap = {
        lockButton: lock,
        musicButton: getMusic,
        cupLocationButton: location
    };

    menuButtons.forEach(buttonId => {
        document.getElementById(buttonId).addEventListener('click', function(event) {
            // If this button is already active, remove the active class
            document.querySelectorAll('.options').forEach(option => {
                option.classList.add('hidden');
            });

            if (event.currentTarget.classList.contains('button-active')) {
                event.currentTarget.classList.remove('button-active');
            } else {
                // Otherwise, make this the only active button
                menuButtons.forEach(id => {
                    document.getElementById(id).classList.remove('button-active');
                });
                event.currentTarget.classList.add('button-active');

            const functionToCall = functionMap[event.currentTarget.id];
            if (functionToCall) {
                functionToCall();
                sendData(event.currentTarget.id); 
            }

            }   
        });
    });


    document.getElementById('themeToggle').addEventListener('click', function() {
        var body = document.body;
        if (body.classList.contains('neon-theme')) {
            body.classList.remove('neon-theme');
        }
        body.classList.toggle('dark-theme');
        if (body.classList.contains('dark-theme')) {
            body.classList.remove('light-theme');
        } else {
            body.classList.add('light-theme');
        }
    });
    


    document.getElementById('neonToggle').addEventListener('click', function() {
        var body = document.body;
        if (body.classList.contains('dark-theme') || body.classList.contains('light-theme')) {
            body.classList.remove('dark-theme');
            body.classList.remove('light-theme');
        }
        body.classList.toggle('neon-theme');
        var neonBackground = document.querySelector('.neon-background');
        if(neonBackground.style.display === 'none' || neonBackground.style.display === '') {
            neonBackground.style.display = 'block';
        } else {
            neonBackground.style.display = 'none';
        }
    });

    


    function getMusic() {
        console.log("getMusic fx");
        var musicOptions = document.getElementById('musicOptions');
        if(musicOptions) {
            document.querySelectorAll('.options').forEach(function(option) {
                option.classList.add('hidden');
            });
            musicOptions.classList.remove('hidden');
        }
    }
    
    function location() {
        console.log("location fx");
        var cupLocationOptions = document.getElementById('cupLocationOptions');
        if(cupLocationOptions) {
            document.querySelectorAll('.options').forEach(function(option) {
                option.classList.add('hidden');
            });
            cupLocationOptions.classList.remove('hidden');
            goodLock.classList.add('selected');
            badLock.classList.remove('red-glow');
        }




    }
    
    function lock() {
        console.log("lock fx");
        var lockOptions = document.getElementById('lockOptions');
        if(lockOptions) {
            document.querySelectorAll('.options').forEach(function(option) {
                option.classList.add('hidden');


            });
            lockOptions.classList.remove('hidden');
            goodLock.classList.add('selected');
            badLock.classList.remove('red-glow');
        }
    }
    
    function sendData(buttonId) {
        fetch(`/state_change/?param1=${buttonId}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
        })
        .then(response => {
            if (!response.ok) { throw new Error('Network response was not ok'); }
            return response.json(); // or .text() if you don't need a JSON response
        })
        .then(data => {
            console.log('Success:', data);
        })
        .catch((error) => {
            console.error('Error:', error);
        });
    }





    console.log("Initialized!");
    fetchData();
});

// Poll the server every 2.5 seconds
setInterval(fetchData, 2500);

function fetchData() {
  fetch('/current_state', { method: 'GET' })
    .then(response => {
      if (!response.ok) { throw new Error('Network response was not ok'); }
      return response.json(); // Parse response as JSON
    })
    .then(data => {
      console.log("Updated state:", data);
      updateUI(data); // Function to handle UI updates based on state
      
      })
    .catch(error => {
      console.error("Error fetching state:", error);
    });
}

function updateUI(data) {
  const goodLock = document.getElementById("goodLock");
  const badLock = document.getElementById("badLock");

  if (data.curSubState === 'GOOD') {

  } else if (data.curSubState === 'BAD') {
    badLock.classList.add('red-glow');
    goodLock.classList.remove('selected');
  }
}