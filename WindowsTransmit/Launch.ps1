# 3. & 4. Auto-Detect the Correct Bluetooth Port
Write-Host "Scanning for ESP32 Bluetooth ports..." -ForegroundColor Cyan

# Get all ports that look like Bluetooth standard serial links
$potentialPorts = Get-PnpDevice -Class Ports -Present | 
    Where-Object { $_.FriendlyName -match 'Standard Serial' -or $_.FriendlyName -match 'Bluetooth' }

$validPort = $null

if ($potentialPorts.Count -eq 0) {
    Write-Host "No Bluetooth ports found. Is the ESP32 paired?" -ForegroundColor Red
    Pause
    exit
}

# Iterate through each port and test it using a quick Python connection
foreach ($p in $potentialPorts) {
    if ($p.FriendlyName -match '\((COM\d+)\)') {
        $testPort = $matches[1]
        Write-Host "Testing $testPort... " -NoNewline
        
        # We use a tiny python command to try opening the port for 1 second
        $command = "import serial; s = serial.Serial('$testPort', baudrate=115200, timeout=1); s.close()"
        
        try {
            # Run the test command via Python, suppressing error output
            python -c $command 2>$null
            
            # Check the exit code of the last command
            if ($LASTEXITCODE -eq 0) {
                Write-Host "Success!" -ForegroundColor Green
                $validPort = $testPort
                break # We found it, stop searching
            } else {
                Write-Host "Failed (Likely 'Incoming' port or device off)" -ForegroundColor DarkGray
            }
        } catch {
            Write-Host "Error" -ForegroundColor Red
        }
    }
}

if ([string]::IsNullOrWhiteSpace($validPort)) {
    Write-Host "`nCould not automatically connect to any ESP32 port." -ForegroundColor Red
    Write-Host "Ensure the ESP32 is POWERED ON and PAIRED."
    Pause
    exit
}

Write-Host "Target Locked: $validPort" -ForegroundColor Green
Write-Host "----------------------------------------"
Write-Host "Launching Data Reader..." -ForegroundColor Cyan

# --- NEW CODE START ---
# Launch Read.py and pass the found port as an argument
python Read.py $validPort
# --- NEW CODE END ---