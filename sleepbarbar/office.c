int chairs       = N # available chairs
bool is_sleeping = false
bool is_cutting  = false 
bool finished    = false # All customers taken care of (Changed in main)

# Semaphores for read/write access
semaphore rw_chairs   = 1
semaphore rw_sleeping = 1
semaphore rw_cutting  = 1

semaphore barber_ready = 0 # Barber ready to cut hair
semaphore sleeping     = 0 # Barber is sleeping
semaphore cutting      = 0 # Barber cutting a customer's hair

def Barber():
   while not finished:
      wait(rw_chairs)
      wait(rw_sleeping)
      # If no chairs are being used
      if chairs == N:
         is_sleeping = true
         print("Barber is sleeping!")
         signal(rw_chairs)     # Allow others to read/write
         signal(rw_sleeping)
         wait(sleeping)        # Wait for customer to wake him up
         signal(barber_ready)  # Allow customers into the chair
         print("Barber woke up!")
      else:
         signal(rw_signal)
         chairs += 1
         signal(barber_ready)
         signal(rw_chairs)
      # If the barber isn't done for the day, help the customer
      if not finished:
         print("Helping a customer")
         # Wait a random amount of time
         print("Finished helping a customer")
         signal(cutting) # Let the customer leave after the hair cut
      else:
         print("Barber is done for the day!")

def Customer():
   bool helped = false
   while not helped:
      wait(rw_chairs)
      wait(rw_cutting)
      if chairs == N and not is_cutting: # If the barber is free
         wait(rw_sleeping)
         if is_sleeping:
            signal(sleeping) # Wake the barber up
            is_sleeping = false
            print("Customer has woken the barber up")
         signal(rw_sleeping)
         is_cutting = true
         signal(rw_chairs)
         signal(rw_cutting)
         wait(barber_ready) # Wait for the barber to be ready
         wait(cutting)      # Wait for him to finish cutting hair
         print("Customer has had his hair cut")
         helped = true
      else if chairs > 0:
         chairs -= 1
         print("Customer is waiting in a chair")
         signal(rw_chairs)
         signal(rw_cutting)
         wait(barber_ready)
         wait(cutting)
         print("Customer has had his hair cut")
         helped = true
      else:
         signal(rw_chairs)
         signal(rw_office)
         print("All chairs taken, will return later")
         # Wait a random amount of time 
   print("Customer is leaving the barbershop")
