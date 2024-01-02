<script lang="ts">
    import lecturers from '../routes/lecturer/lecturer.json';
    import Tags from './Tags.svelte';
    import Contact from './Contact.svelte';
    const lec = Array.isArray(lecturers) ? lecturers : [lecturers];
    const { tags } = lecturers;
    const { contact } = lecturers;

</script>
  
<style>
    
</style>
  
  <div class="w-3/4 h-5/6 flex flex-col justify-evenly items-center">
    {#each lec as lecturer (lecturer.first_name)} 

        <img src={lecturer.picture_url} alt={lecturer.last_name} class="rounded-2xl min-h-20">

      
        <p class="text-xl font-bold">jméno: <span>{lecturer.title_before} </span>{lecturer.first_name} <span>{lecturer.middle_name}</span> <span>{lecturer.last_name}</span> <span>{lecturer.title_after}</span></p>
        <p class="">{lecturer.claim}</p>
        <p class="">město: {lecturer.location}</p>
        
        <p class="">{@html lecturer.bio}</p>
        <div class="w-full flex flex-wrap justify-evenly ">
          <Tags {tags}/>
        </div>
        <p class="self-start">cena za hodinu: {lecturer.price_per_hour},-</p>
        <div>
          <Contact {contact}/>
        </div>
        <p class="self-end opacity-50">id lektora: {lecturer.UUID}</p>
    {/each}
  </div>

