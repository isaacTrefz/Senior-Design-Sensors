# 1. Check if Python is installed
try {
    $pythonVersion = python --version 2>&1
    Write-Host "Found $pythonVersion" -ForegroundColor Green
} catch {
    Write-Host "Python is not installed or not in your PATH." -ForegroundColor Red
    Write-Host "Please install Python from python.org and check 'Add Python to PATH' during installation."
    Pause
    exit
}

# 2. Install the required library (pyserial) automatically
Write-Host "Checking/Installing required library (pyserial)..." -ForegroundColor Cyan
pip install pyserial | Out-Null

# 3. Find Bluetooth COM Ports
Write-Host "Scanning for Bluetooth COM ports..." -ForegroundColor Cyan
# Get devices that look like Serial ports
$ports = Get-PnpDevice -Class Ports -Present | Where-Object { $_.FriendlyName -match '\(COM' }

# Filter for Bluetooth if possible, or list all standard ports
$bluetoothPorts = $ports | Where-Object { $_.FriendlyName -match 'Bluetooth' -or $_.FriendlyName -match 'Standard Serial' }

if ($bluetoothPorts.Count -eq 0) {
    Write-Host "No Bluetooth serial ports found!" -ForegroundColor Red
    Write-Host "Make sure your ESP32 is paired to Windows in Settings > Devices."
    Pause
    exit
}

# 4. Select the Port
$selectedPort = ""
if ($bluetoothPorts.Count -eq 1) {
    # If only one Bluetooth port is found, verify it's the right one (outgoing)
    $p = $bluetoothPorts[0]
    # Extract COM number (e.g., COM4)
    if ($p.FriendlyName -match '\((COM\d+)\)') {
        $selectedPort = $matches[1]
    }
    Write-Host "Auto-detected Bluetooth Device on $selectedPort" -ForegroundColor Green
} else {
    # If multiple ports, ask the user to pick one
    Write-Host "Multiple Bluetooth ports found. Usually, the 'Outgoing' one is correct." -ForegroundColor Yellow
    for ($i = 0; $i -lt $bluetoothPorts.Count; $i++) {
        Write-Host "[$i] $($bluetoothPorts[$i].FriendlyName)"
    }
    $selection = Read-Host "Enter the number of your device (e.g., 0 or 1)"
    if ($bluetoothPorts[$selection].FriendlyName -match '\((COM\d+)\)') {
        $selectedPort = $matches[1]
    }
}

if ([string]::IsNullOrWhiteSpace($selectedPort)) {
    Write-Host "Could not determine the COM port." -ForegroundColor Red
    Pause
    exit
}

# 5. Modify Read.py to use the selected port
$pythonFile = ".\Read.py"
if (Test-Path $pythonFile) {
    $content = Get-Content $pythonFile
    # Regex to find SERIAL_PORT = '...' and replace it
    $newContent = $content -replace "SERIAL_PORT\s*=\s*['`"].*['`"]", "SERIAL_PORT = '$selectedPort'"
    $newContent | Set-Content $pythonFile
    Write-Host "Configured Read.py to use $selectedPort" -ForegroundColor Green
} else {
    Write-Host "Could not find Read.py in this folder!" -ForegroundColor Red
    Pause
    exit
}

# 6. Run the Python Script
Write-Host "Starting Data Stream..." -ForegroundColor Cyan
Write-Host "Press Ctrl+C to stop."
python Read.py